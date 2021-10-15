#include "tree_sitter/parser.h"
#include <array>
#include <algorithm>
#include <locale>
#include <iostream>
#include <regex>
#include <string>
#include <cwctype>

enum TokenType
{
    NONE,

    SPACE,

    WORD,
    CAPITALIZED_WORD,

    PARENTHESIZED_TEXT,

    LINE_BREAK,
    PARAGRAPH_BREAK,

    ESCAPE_SEQUENCE,

    HEADING1,
    HEADING2,
    HEADING3,
    HEADING4,
    HEADING5,
    HEADING6,

    QUOTE1,
    QUOTE2,
    QUOTE3,
    QUOTE4,
    QUOTE5,
    QUOTE6,

    UNORDERED_LIST1,
    UNORDERED_LIST2,
    UNORDERED_LIST3,
    UNORDERED_LIST4,
    UNORDERED_LIST5,
    UNORDERED_LIST6,

    ORDERED_LIST1,
    ORDERED_LIST2,
    ORDERED_LIST3,
    ORDERED_LIST4,
    ORDERED_LIST5,
    ORDERED_LIST6,

    MARKER,

    TODO_ITEM_UNDONE,
    TODO_ITEM_PENDING,
    TODO_ITEM_DONE,

    INSERTION,

    UNORDERED_LINK1,
    UNORDERED_LINK2,
    UNORDERED_LINK3,
    UNORDERED_LINK4,
    UNORDERED_LINK5,
    UNORDERED_LINK6,

    ORDERED_LINK1,
    ORDERED_LINK2,
    ORDERED_LINK3,
    ORDERED_LINK4,
    ORDERED_LINK5,
    ORDERED_LINK6,

    STRONG_PARAGRAPH_DELIMITER,
    WEAK_PARAGRAPH_DELIMITER,
    HORIZONTAL_LINE,

    LINK_BEGIN,
    LINK_END_GENERIC,
    LINK_END_URL,
    LINK_END_HEADING1_REFERENCE,
    LINK_END_HEADING2_REFERENCE,
    LINK_END_HEADING3_REFERENCE,
    LINK_END_HEADING4_REFERENCE,
    LINK_END_HEADING5_REFERENCE,
    LINK_END_HEADING6_REFERENCE,
    LINK_END_MARKER_REFERENCE,

    RANGED_TAG,
    RANGED_TAG_END,

    CARRYOVER_TAG,

    SINGLE_DEFINITION,
    MULTI_DEFINITION,
    MULTI_DEFINITION_SUFFIX,

    BOLD,
    ITALIC,
    STRIKETHROUGH,
    UNDERLINE,
    SPOILER,
    INLINE_CODE,
    SUPERSCRIPT,
    SUBSCRIPT,

    BOLD_WITH_NEST,
    ITALIC_WITH_NEST,
    STRIKETHROUGH_WITH_NEST,
    UNDERLINE_WITH_NEST,
    SPOILER_WITH_NEST,
    INLINE_CODE_WITH_NEST,
    SUPERSCRIPT_WITH_NEST,
    SUBSCRIPT_WITH_NEST,

    MARKUP_END,
};

// Operator overloads for TokenTypes (allows for their chaining)
namespace
{
    std::vector<TokenType> operator|(TokenType lhs, TokenType rhs)
    {
        return std::vector<TokenType>({ lhs, static_cast<TokenType>(rhs) });
    }

    std::vector<TokenType>&& operator|(std::vector<TokenType>&& lhs, TokenType rhs)
    {
        lhs.push_back(rhs);
        return std::move(lhs);
    }
}

class Scanner
{
public:
    bool scan(TSLexer* lexer, const bool* valid_symbols)
    {
        lexer->result_symbol = NONE;

        // Are we at the end of file? If so, bail
        if (lexer->eof(lexer) || !lexer->lookahead)
        {
            advance(lexer);
            return false;
        }

        // TODO: Remove the insane amount of m_TagStack.empty() checks and put it all
        // in one if check

        // Check for an escape seqence (e.g. "\*")
        if (m_TagStack.empty() && lexer->lookahead == '\\')
        {
            advance(lexer);

            if (lexer->lookahead)
            {
                lexer->result_symbol = m_LastToken = ESCAPE_SEQUENCE;
                return true;
            }
            else
                return false;
        }

        // If we are not in a tag and we have a square bracket opening then try matching
        // either a todo item or beginning of a link
        if (m_TagStack.empty() && lexer->lookahead == '[')
        {
            advance(lexer);

            // If we instantly close the bracket (i.e. "[]") then treat it as an empty link beginning
            if (lexer->lookahead == ']')
            {
                lexer->result_symbol = m_LastToken = WORD;
                return true;
            }

            // Move over any whitespace
            while (lexer->lookahead == ' ' || lexer->lookahead == '\t')
                advance(lexer);

            switch (lexer->lookahead)
            {
                // Did we encounter the end bracket? We've just dealt with an undone todo item
                // ([ ])
                case ']':
                    advance(lexer);
                    lexer->result_symbol = m_LastToken = TODO_ITEM_UNDONE;
                    return true;
                // We're dealing with a pending item ([*])
                case '*':
                    lexer->result_symbol = m_LastToken = TODO_ITEM_PENDING;
                    break;
                // We're dealing with a done item ([x])
                case 'x':
                    lexer->result_symbol = m_LastToken = TODO_ITEM_DONE;
                    break;
                // We are simply dealing with a link beginning ([something])
                default:
                    lexer->result_symbol = m_LastToken = LINK_BEGIN;
                    break;
                case '\0':
                    advance(lexer);
                    return false;
            }

            advance(lexer);

            while (lexer->lookahead)
            {
                // If we've encountered an `]` check whether it has been escaped with a backslash
                if (lexer->lookahead == ']' && m_Current != '\\')
                {
                    advance(lexer);
                    return true;
                }
                else if (!std::iswspace(lexer->lookahead) || lexer->lookahead == '\n')
                {
                    lexer->result_symbol = m_LastToken = LINK_BEGIN;
                }

                advance(lexer);
            }

            return lexer->lookahead != 0;
        }
        // Otherwise make sure to check for the existence of an opening link location
        else if (m_TagStack.empty() && lexer->lookahead == '(' && m_Current == ']')
            return check_link(lexer);
        // Otherwise just check whether or not we're dealing with a newline and return PARAGRAPH_BREAK if we are
        else if (lexer->lookahead == '\n')
        {
            advance(lexer);

            lexer->result_symbol = m_LastToken = LINE_BREAK;

            if (lexer->eof(lexer) || !lexer->lookahead)
                return false;

            if (lexer->lookahead == '\n')
            {
                advance(lexer);

                lexer->result_symbol = m_LastToken = PARAGRAPH_BREAK;
            }

            return true;
        }

        // If we're at the beginning of a line check for all detached modifiers
        if (lexer->get_column(lexer) == 0)
        {
            m_IndentationLevel = 0;

            // Skip all leading whitespace and measure the indentation level
            while (lexer->lookahead == ' ' || lexer->lookahead == '\t')
            {
                // If we are dealing with a tab then add an extra level of indentation
                if (lexer->lookahead == '\t')
                    ++m_IndentationLevel;

                ++m_IndentationLevel;

                skip(lexer);
            }

            // We are dealing with a ranged tag (@something)
            if (lexer->lookahead == '@')
            {
                advance(lexer);

                // Mark the end of the token here
                // We do this because we only want the returned token to be part of
                // the `@` symbol, not the symbol + the name
                lexer->mark_end(lexer);

                // These sets of checks check whether the tag is `@end`
                if (lexer->lookahead == 'e')
                {
                    advance(lexer);
                    if (lexer->lookahead == 'n')
                    {
                        advance(lexer);
                        if (lexer->lookahead == 'd')
                        {
                            advance(lexer);
                            if (std::iswspace(lexer->lookahead))
                            {
                                while (std::iswspace(lexer->lookahead) && lexer->lookahead != '\n' && lexer->lookahead)
                                    advance(lexer);

                                if (std::iswspace(lexer->lookahead) && !m_TagStack.empty())
                                {
                                    if (m_IndentationLevel != m_TagStack.back())
                                    {
                                        lexer->result_symbol = m_LastToken = WORD;
                                        return true;
                                    }
                                    else
                                    {
                                        lexer->result_symbol = m_LastToken = RANGED_TAG_END;

                                        m_TagStack.pop_back();
                                        return true;
                                    }
                                }

                                lexer->result_symbol = m_LastToken = WORD;
                                return true;
                            }
                        }
                    }
                }

                // This is a fallback. If the tag ends up not being `@end`
                // then just push back the indentation level and return
                lexer->result_symbol = m_LastToken = RANGED_TAG;
                m_TagStack.push_back(m_IndentationLevel);
                return true;
            }
            else if (lexer->lookahead == '$')
            {
                advance(lexer);
                lexer->result_symbol = m_LastToken = CARRYOVER_TAG;
                return true;
            }

            // Check for these only if we are not inside of a tag ("@example")
            if (m_TagStack.empty())
            {
                // The idea of the check_detached function is as follows:
                // We check for the '*' character and depending on how many we encounter we return a different token
                // If we encounter 1 '*' char then return heading1, if we encounter two in a row then return heading2, you get the idea.
                // If we encounter more than the given amount of fallbacks then the last fallback will always be chosen. This means
                // that if we have 7 consecutive '*' chars then we will still fall back to the HEADING6 token
                if (check_detached(lexer, HEADING1 | HEADING2 | HEADING3 | HEADING4 | HEADING5 | HEADING6, { '*' }) != NONE)
                    return true;

                // Check for the existence of quotes
                if (check_detached(lexer, QUOTE1 | QUOTE2 | QUOTE3 | QUOTE4 | QUOTE5 | QUOTE6 | NONE, { '>' }) != NONE)
                    return true;

                // Check for the existence of an unordered list element.
                // The last parameter tells the neorg parser "hey parse as many '-' chars as possible BUT if you encounter a '>' char at the end of the parsed string then
                // return an UNORDERED_LINK instead".
                if (check_detached(lexer, UNORDERED_LIST1 | UNORDERED_LIST2 | UNORDERED_LIST3 | UNORDERED_LIST4 | UNORDERED_LIST5 | UNORDERED_LIST6 | NONE, { '-' },
                            { '>', UNORDERED_LINK1 | UNORDERED_LINK2 | UNORDERED_LINK3 | UNORDERED_LINK4 | UNORDERED_LINK5 | UNORDERED_LINK6 | NONE }) != NONE)
                {
                    return true;
                }
                // If we end up failing to parse an unordered modifier not all hope is lost.
                // We also have a weak paragraph delimiter which looks like this:
                // ---
                // Detached modifiers are valid only if there is at least one bit of whitespace after themselves.
                // A delimiting modifier strictly does not allow whitespace after itself. That's how we differentiate
                // between the two. m_ParsedChars is incremented every time `check_detached` successfully parses a character.
                // We can use this to our advantage! The parser will encounter 3 consecutive '-' chars and will parse all the way up until
                // the end. It will then try to return UNORDERED_LIST3 but will fail because there won't be any whitespace after the 3 chars.
                // It will then return NONE. Even though it may have failed the m_ParsedChars value has still been modified! If m_ParsedChars is 3
                // then that means we have parsed '---' and hence we return a WEAK_PARAGRAPH_DELIMITER. This check is even further enforced by checking
                // if the next char is a newline, which makes sense considering the parser head:
                // ---
                //   ^ will be here, and lexer->lookahead will return '\n'
                else if (lexer->lookahead == '\n' && m_ParsedChars >= 3)
                {
                    lexer->result_symbol = m_LastToken = WEAK_PARAGRAPH_DELIMITER;
                    return true;
                }

                if (check_detached(lexer, ORDERED_LIST1 | ORDERED_LIST2 | ORDERED_LIST3 | ORDERED_LIST4 | ORDERED_LIST5 | ORDERED_LIST6 | NONE, { '~' },
                            { '>', ORDERED_LINK1 | ORDERED_LINK2 | ORDERED_LINK3 | ORDERED_LINK4 | ORDERED_LINK5 | ORDERED_LINK6 | NONE }) != NONE)
                    return true;

                if (check_detached(lexer, MARKER | NONE, { '|' }) != NONE)
                    return true;

                if (check_detached(lexer, SINGLE_DEFINITION | MULTI_DEFINITION | NONE, { ':' }) != NONE)
                    return true;
                else if (lexer->lookahead == '\n' && m_ParsedChars == 2)
                {
                    lexer->result_symbol = MULTI_DEFINITION_SUFFIX;
                    return true;
                }

                if (check_detached(lexer, INSERTION, { '=' }) != NONE)
                    return true;
                // TODO: Comment
                else if (lexer->lookahead == '\n')
                {
                    if (m_ParsedChars >= 3)
                    {
                        lexer->result_symbol = m_LastToken = STRONG_PARAGRAPH_DELIMITER;
                        return true;
                    }
                    else
                    {
                        advance(lexer);
                        lexer->result_symbol = m_LastToken = WORD;
                        return true;
                    }
                }

                if (check_detached(lexer, NONE, { '_' }) != NONE)
                    return true;
                else if (lexer->lookahead == '\n' && m_ParsedChars >= 3)
                {
                    lexer->result_symbol = HORIZONTAL_LINE;
                    return true;
                }
            }
        }

        // If we are not in a ranged tag then we should also check for potential attached modifiers, like *this*.
        if (m_TagStack.empty() && (check_attached(lexer, false) != NONE))
            return true;

        // Match paragraphs
        return parse_text(lexer);
    }

    std::vector<uint16_t>& get_tag_stack() noexcept { return m_TagStack; }
private:
    // Skips the next character without including it in the final result
    void skip(TSLexer* lexer)
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        return lexer->advance(lexer, true);
    }

    // Advances the lexer forward. The char that was advanced will be returned in the final result
    void advance(TSLexer* lexer)
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        return lexer->advance(lexer, false);
    }

    // An alternate implementation for `check_detached` in case you don't have a bunch of chained results
    template <size_t Size = 1>
    inline TokenType check_detached(TSLexer* lexer, TokenType result, const std::array<int32_t, Size>& expected, std::pair<char, TokenType> terminate_at = { 0, NONE })
    {
        return check_detached(lexer, result | NONE, expected, { terminate_at.first, terminate_at.second | NONE });
    }

    /*
     * Checks for the existence of a detached modifier
     * @param lexer - a pointer to the treesitter lexer
     * @param results - a list of potential results repending on the amount of consecutive matches found
     * @param expected - a list of expected modifiers to appear in the sequence
     */
    template <size_t Size = 1>
    [[nodiscard]]
    TokenType check_detached(TSLexer* lexer, const std::vector<TokenType>& results, const std::array<int32_t, Size>& expected, std::pair<char, std::vector<TokenType>> terminate_at = { 0, NONE | NONE })
    {
        static_assert(Size > 0, "check_detached Size template must be greater than 0");

        size_t i = m_ParsedChars = 0;

        // Loop as long as the next character is a valid detached modifier
        for (auto detached_modifier = std::find(s_DetachedModifiers.begin(), s_DetachedModifiers.end(), lexer->lookahead);
                detached_modifier != s_DetachedModifiers.end();
                detached_modifier = std::find(s_DetachedModifiers.begin(), s_DetachedModifiers.end(), lexer->lookahead), i++, m_ParsedChars++)
        {
            // If we've specified a termination character and we match then the token lexing prematurely
            if (terminate_at.first != 0 && lexer->lookahead == terminate_at.first)
            {
                advance(lexer);

                // Skip other potential whitespace
                while (lexer->lookahead && (lexer->lookahead == ' ' || lexer->lookahead == '\t'))
                    advance(lexer);

                TokenType result = terminate_at.second[clamp(i, size_t{}, terminate_at.second.size()) - 1];

                lexer->result_symbol = m_LastToken = result;

                return result;
            }

            // If the next character is not one we expect then break
            // We use clamp() here to prevent overflow and to make the last element of the expected array the fallback
            if (lexer->lookahead != expected[clamp(i, size_t{}, Size - 1)])
                break;

            advance(lexer);

            // If the next character is whitespace (which is the distinguishing factor between an attached/detached modifier)
            if (std::iswspace(lexer->lookahead) && (lexer->lookahead != '\n'))
            {
                // Retrieve the correct result from the list of provided results depending on how many characters were matched.
                // If we've exceeded the number of results then the clamp function will fall back to the last element
                    TokenType result = results[clamp(i, size_t{}, results.size() - 1)];

                // Skip any other potential whitespace
                while (lexer->lookahead && (lexer->lookahead == ' ' || lexer->lookahead == '\t'))
                    advance(lexer);

                lexer->result_symbol = m_LastToken = result;

                return result;
            }
        }

        // If we've only parsed one character and instantly failed then we might be dealing with
        // an attached modifier!
        if (m_ParsedChars == 1)
            return check_attached(lexer, true);
        else
            return NONE;
    }

    auto find_attached(int32_t c)
    {
        return std::find_if(s_AttachedModifiers.begin(), s_AttachedModifiers.end(), [&](const std::pair<int32_t, TokenType>& pair) { return pair.first == c; });
    }

    /*
     * Checks for the existence of an attached modifier
     * @param lexer - a pointer to the treesitter lexer
     * @param behind - if true the parser will use m_Current instead of lexer->lookahead as its main lookahead source
     */
    TokenType check_attached(TSLexer* lexer, bool behind)
    {
        // Bind the `lookahead` and `current` values
        int32_t& lookahead = behind ? m_Current : lexer->lookahead;
        int32_t& current = behind ? m_Previous : m_Current;

        // Return an iterator to an attached modifier if one can be found
        const auto attached_modifier = m_AttachedModifierStack.empty() ? find_attached(lookahead) : &m_AttachedModifierStack.back();

        // std::cout << (char)current << ", " << (char)lookahead << ", " << char(!m_AttachedModifierStack.empty() ? m_AttachedModifierStack.back().first : 0) << std::endl;

        // This will advance the lexer forward only if `behind` is false
        // If it's true then we may end up accidentally advancing too far
        auto conditional_advance = [&](TSLexer* lx) {
            if (!behind)
            {
                advance(lx);
                ++m_ParsedChars;
            }
        };

        if (!m_AttachedModifierStack.empty())
        {
            advance(lexer);

            if (find_attached(m_Current) != s_AttachedModifiers.end() && (std::iswspace(lexer->lookahead) || std::ispunct(lexer->lookahead)))
            {
                m_AttachedModifierStack.pop_back();

                if (m_AttachedModifierStack.empty() && m_NestedModifiers)
                {
                    m_NestedModifiers = false;
                    lexer->result_symbol = m_LastToken = MARKUP_END;
                    return m_LastToken;
                }

                lexer->result_symbol = m_LastToken = attached_modifier->second;
                return m_LastToken;
            }

            goto parse_until_end;
        }

        // Is our iterator valid? If it is then that means we've found an opening attached modifier
        if (attached_modifier != s_AttachedModifiers.end())
        {
            advance(lexer);

            // If the next char is a whitespace character then it doesn't count 
            if (std::iswspace(lookahead))
                return NONE;

            // If we have another attached modifier of the same type right after then do not count it
            // e.g. if our input is "**" it'll get discarded and treated as a PARAGRAPH_SEGMENT instead
            if (lookahead == attached_modifier->first)
            {
                conditional_advance(lexer);
                return NONE;
            }
            m_AttachedModifierStack.push_back(*attached_modifier);

        parse_until_end:
            lexer->mark_end(lexer);

            // While our lookahead is not equal to a potential closing modifier
            while (lookahead != attached_modifier->first || current == '\\')
            {
                auto attached = find_attached(lookahead);

                if (attached_modifier->second != INLINE_CODE && (std::iswspace(current) || std::ispunct(current)) && attached != s_AttachedModifiers.end())
                {
                    m_NestedModifiers = true;

                    lexer->mark_end(lexer);

                    lexer->result_symbol = m_LastToken = static_cast<TokenType>(attached_modifier->second + 8);
                    m_AttachedModifierStack.push_back(*attached);
                    return m_LastToken;
                }

                // If we've encounted the end of our file then bail
                if (!lookahead)
                    return NONE;
                else if (lookahead == '\n')
                {
                    conditional_advance(lexer);

                    // If this check succeeds then we've encountered a \n\n sequence
                    // and as a consequence should terminate the attached modifier
                    if (lookahead == '\n')
                    {
                        m_AttachedModifierStack.clear();

                        lexer->result_symbol = m_LastToken = (bool)std::iswupper(lexer->lookahead) ? CAPITALIZED_WORD : WORD;
                        lexer->mark_end(lexer);
                        return attached_modifier->second;
                    }
                }

                advance(lexer);
            }

            // If the previous char before the closing modifier is not whitespace
            // then
            if (!current || std::isalnum(current) || std::ispunct(current))
            {
                conditional_advance(lexer);

                // If the next char is whitespace then we've successfully matched our modifier!
                if (std::iswspace(lookahead) || std::ispunct(lookahead))
                {
                    // Mark the end of this sequence
                    // We do this as to not accidentally read past EOF
                    lexer->mark_end(lexer);

                    // If the next character is a newline then
                    // read the newline. We do this because otherwise
                    // the parser bugs out and starts a new paragraph
                    if (lexer->lookahead == '\n')
                    {
                        advance(lexer);

                        // If the next char is not EOF then
                        // expand the token to include the newline too
                        if (lexer->lookahead)
                            lexer->mark_end(lexer);
                    }

                    m_AttachedModifierStack.pop_back();

                    if (m_AttachedModifierStack.empty() && m_NestedModifiers)
                    {
                        m_NestedModifiers = false;
                        lexer->result_symbol = m_LastToken = MARKUP_END;
                        return m_LastToken;
                    }

                    lexer->result_symbol = m_LastToken = attached_modifier->second;
                    return attached_modifier->second;
                }
            }
        }

        return NONE;
    }

    /*
     * Attempts to parse a link ([like](#this))
     */
    bool check_link(TSLexer* lexer)
    {
        advance(lexer);

        if (lexer->lookahead == ':')
        {
            while (lexer->lookahead && lexer->lookahead != '*' && lexer->lookahead != '#' && lexer->lookahead != '|' && lexer->lookahead != ')')
                advance(lexer);
            if (m_Current != ':')
                return true;
        }

        advance(lexer);

        // Is the current char an asterisk? We're dealing with a heading reference.
        if (m_Current == '*')
        {
            size_t heading_level = 0;

            // Keep capturing asterisks and increment the heading level accordingly
            while (lexer->lookahead && lexer->lookahead == '*')
            {
                advance(lexer);
                ++heading_level;
            }

            // We use the clamp() here to make sure we don't overflow!
            lexer->result_symbol = m_LastToken = static_cast<TokenType>(LINK_END_HEADING1_REFERENCE + clamp(heading_level, size_t{}, size_t{5}));
        }
        // We're dealing with a marker reference
        else if (m_Current == '|')
        {
                lexer->result_symbol = m_LastToken = LINK_END_MARKER_REFERENCE;
        }
        // We're dealing with a generic (loose) link
        else if (m_Current == '#')
            lexer->result_symbol = m_LastToken = LINK_END_GENERIC;

        // Until we don't hit the end of the link location keep advancing
        while (lexer->lookahead && lexer->lookahead != ')')
        {
            if (lexer->lookahead == '\n' || !lexer->lookahead)
            {
                lexer->result_symbol = m_LastToken = LINK_END_GENERIC;
                break;
            }

            advance(lexer);

            if (m_Current == '\\')
            {
                advance(lexer);
                advance(lexer);
                continue;
            }
            // This is our method of checking for a URL
            // If there's a :// in the string we just assume that it's a URL and that's it
            else if (m_Current == ':' && lexer->lookahead == '/' && lexer->result_symbol == NONE)
            {
                advance(lexer);
                if (lexer->lookahead == '/')
                    lexer->result_symbol = m_LastToken = LINK_END_URL;
            }
        }

        advance(lexer);

        if (lexer->result_symbol == NONE)
            lexer->result_symbol = m_LastToken = PARENTHESIZED_TEXT;

        return true;
    }

    /*
     * Simply parses any line (also called a paragraph segment)
     */
    bool parse_text(TSLexer* lexer)
    {
        if (!m_TagStack.empty())
        {
            while (lexer->lookahead && lexer->lookahead != '\n')
                advance(lexer);

            lexer->result_symbol = m_LastToken = WORD;
            return true;
        }

        if (lexer->lookahead == ' ' || lexer->lookahead == '\t')
        {
            do
                advance(lexer);
            while (lexer->lookahead && std::iswblank(lexer->lookahead));

            lexer->result_symbol = m_LastToken = SPACE;
            return true;
        }

        const TokenType resulting_symbol = (bool)std::iswupper(lexer->lookahead) ? CAPITALIZED_WORD : WORD;

        do
        {
            if (lexer->lookahead == '~')
            {
                advance(lexer);

                if (lexer->lookahead == '\n')
                {
                    advance(lexer);

                    if (lexer->eof(lexer) || !lexer->lookahead)
                        return false;
                }
            }
            else
                advance(lexer);
        }
        while (lexer->lookahead && !std::iswspace(lexer->lookahead) && lexer->lookahead != '\\'); // TODO: Perform specific checks for attached modifiers

        lexer->result_symbol = m_LastToken = resulting_symbol;
        return true;
    }

    template<typename T, typename Comp>
    T clamp(T value, Comp min, Comp max)
    {
        return value < min ? min : (value > max ? max : value);
    }

private:
    // Stores the current char rather than the next char
    int32_t m_Previous = 0, m_Current = 0;

    // The last matched token type (used to detect things like todo items
    // which require an unordered list prefix beforehand)
    TokenType m_LastToken = NONE;

    // Used for lookback
    size_t m_ParsedChars = 0, m_IndentationLevel = 0;

    // Used for tags and things like *bold*
    std::vector<uint16_t> m_TagStack;
    std::vector<std::pair<int32_t, TokenType>> m_AttachedModifierStack;
    bool m_NestedModifiers = false;

private:
    const std::array<int32_t, 8> s_DetachedModifiers = { '*', '-', '>', '|', '=', '~', ':', '_' };
    const std::array<std::pair<int32_t, TokenType>, 8> s_AttachedModifiers = { std::pair<int32_t, TokenType> { '*', BOLD }, { '-', STRIKETHROUGH }, { '_', UNDERLINE }, { '/', ITALIC }, { '|', SPOILER }, { '^', SUPERSCRIPT }, { ',', SUBSCRIPT }, { '`', INLINE_CODE } };
};

extern "C"
{
    void* tree_sitter_norg_external_scanner_create()
    {
        return new Scanner();
    }

    void tree_sitter_norg_external_scanner_destroy(void* payload)
    {
        delete static_cast<Scanner*>(payload);
    }

    bool tree_sitter_norg_external_scanner_scan(void* payload, TSLexer* lexer, const bool* valid_symbols)
    {
        return static_cast<Scanner*>(payload)->scan(lexer, valid_symbols);
    }

    unsigned tree_sitter_norg_external_scanner_serialize(void* payload, char* buffer)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);

        auto& tag_stack = scanner->get_tag_stack();

        if (tag_stack.size() >= TREE_SITTER_SERIALIZATION_BUFFER_SIZE)
            return 0;

        std::copy(tag_stack.begin(), tag_stack.end(), buffer);

        return tag_stack.size();
    }

    void tree_sitter_norg_external_scanner_deserialize(void* payload, const char* buffer, unsigned length)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);

        auto& tag_stack = scanner->get_tag_stack();

        tag_stack.clear();
        tag_stack.resize(length);

        std::copy_n(buffer, length, tag_stack.begin());
    }
}
