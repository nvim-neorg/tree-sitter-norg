#include <algorithm>
#include <array>
#include <cwctype>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <unordered_map>

#include "tree_sitter/parser.h"

enum TokenType : char
{
    NONE,

    SPACE,

    WORD,
    CAPITALIZED_WORD,

    LINE_BREAK,
    PARAGRAPH_BREAK,

    ESCAPE_SEQUENCE,

    TRAILING_MODIFIER,

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
    TODO_ITEM_ON_HOLD,
    TODO_ITEM_CANCELLED,
    TODO_ITEM_URGENT,
    TODO_ITEM_UNCERTAIN,
    TODO_ITEM_RECURRING,

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
    LINK_FILE_BEGIN,
    LINK_FILE_TEXT,
    LINK_FILE_END,
    LINK_LOCATION_GENERIC,
    LINK_LOCATION_URL,
    LINK_LOCATION_EXTERNAL_FILE,
    LINK_LOCATION_HEADING1,
    LINK_LOCATION_HEADING2,
    LINK_LOCATION_HEADING3,
    LINK_LOCATION_HEADING4,
    LINK_LOCATION_HEADING5,
    LINK_LOCATION_HEADING6,
    LINK_LOCATION_MARKER,
    LINK_LOCATION_DEFINITION,
    LINK_LOCATION_FOOTNOTE,
    LINK_LOCATION_TEXT,
    LINK_END,
    LINK_TEXT_BEGIN,
    LINK_TEXT,
    LINK_TEXT_END,

    ANCHOR_DECLARATION_BEGIN,
    ANCHOR_DECLARATION_TEXT,
    ANCHOR_DECLARATION_END,

    RANGED_TAG,
    RANGED_TAG_END,

    CARRYOVER_TAG,

    SINGLE_DEFINITION,
    MULTI_DEFINITION,
    MULTI_DEFINITION_SUFFIX,

    SINGLE_FOOTNOTE,
    MULTI_FOOTNOTE,
    MULTI_FOOTNOTE_SUFFIX,

    LINK_MODIFIER,

    BOLD_OPEN,
    ITALIC_OPEN,
    STRIKETHROUGH_OPEN,
    UNDERLINE_OPEN,
    SPOILER_OPEN,
    VERBATIM_OPEN,
    SUPERSCRIPT_OPEN,
    SUBSCRIPT_OPEN,
    INLINE_COMMENT_OPEN,
    INLINE_MATH_OPEN,
    VARIABLE_OPEN,

    SCOPED_BOLD_OPEN,
    SCOPED_ITALIC_OPEN,
    SCOPED_STRIKETHROUGH_OPEN,
    SCOPED_UNDERLINE_OPEN,
    SCOPED_SPOILER_OPEN,
    SCOPED_VERBATIM_OPEN,
    SCOPED_SUPERSCRIPT_OPEN,
    SCOPED_SUBSCRIPT_OPEN,
    SCOPED_INLINE_COMMENT_OPEN,
    SCOPED_INLINE_MATH_OPEN,
    SCOPED_VARIABLE_OPEN,

    MARKUP_CLOSE,
};

// Operator overloads for TokenTypes (allows for their chaining)
namespace
{
std::vector<TokenType> operator|(TokenType lhs, TokenType rhs)
{
    return std::vector<TokenType>({lhs, static_cast<TokenType>(rhs)});
}

std::vector<TokenType>&& operator|(std::vector<TokenType>&& lhs, TokenType rhs)
{
    lhs.push_back(rhs);
    return std::move(lhs);
}
}  // namespace

template <typename Type>
std::ostream& operator<<(std::ostream& stream, const std::vector<Type>& vec)
{
    stream << '{';

    for (auto& elem : vec)
    {
        stream << elem << ",";
    }

    return stream << '}';
}

template <typename First, typename Second>
std::ostream& operator<<(std::ostream& stream, const std::pair<First, Second>& pair)
{
    return stream << '(' << (char)pair.first << ", " << (int)pair.second << ')';
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

        // Otherwise just check whether or not we're dealing with a newline and
        // return STANDALONE_BREAK if we are
        if (m_LastToken == TRAILING_MODIFIER)
        {
            advance(lexer);
            return parse_text(lexer);
        }
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
            // Skip all leading whitespace and measure the indentation level
            while (std::iswblank(lexer->lookahead))
                skip(lexer);

            // We are dealing with a ranged tag (@something)
            if (lexer->lookahead == '@')
            {
                advance(lexer);

                // Mark the end of the token here
                // We do this because we only want the returned token to be part
                // of the `@` symbol, not the symbol + the name
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
                                while (std::iswspace(lexer->lookahead) &&
                                       lexer->lookahead != '\n' && lexer->lookahead)
                                    advance(lexer);

                                if (std::iswspace(lexer->lookahead) && m_TagLevel)
                                {
                                    lexer->result_symbol = m_LastToken = RANGED_TAG_END;

                                    --m_TagLevel;
                                    return true;
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
                ++m_TagLevel;
                return true;
            }
            else if (lexer->lookahead == '#')
            {
                advance(lexer);
                lexer->result_symbol = m_LastToken = CARRYOVER_TAG;
                return true;
            }

            if (m_TagLevel)
                return parse_text(lexer);

            // The idea of the check_detached function is as follows:
            // We check for the '*' character and depending on how many we
            // encounter we return a different token If we encounter 1 '*'
            // char then return heading1, if we encounter two in a row then
            // return heading2, you get the idea. If we encounter more than
            // the given amount of fallbacks then the last fallback will
            // always be chosen. This means that if we have 7 consecutive
            // '*' chars then we will still fall back to the HEADING6 token
            if (check_detached(lexer,
                               HEADING1 | HEADING2 | HEADING3 | HEADING4 | HEADING5 | HEADING6,
                               {'*'}) != NONE)
                return true;

            // Check for the existence of quotes
            if (check_detached(lexer, QUOTE1 | QUOTE2 | QUOTE3 | QUOTE4 | QUOTE5 | QUOTE6 | NONE,
                               {'>'}) != NONE)
                return true;

            // Check for the existence of an unordered list element.
            // The last parameter tells the neorg parser "hey parse as many
            // '-' chars as possible BUT if you encounter a '>' char at the
            // end of the parsed string then return an UNORDERED_LINK
            // instead".
            if (check_detached(
                    lexer,
                    UNORDERED_LIST1 | UNORDERED_LIST2 | UNORDERED_LIST3 | UNORDERED_LIST4 |
                        UNORDERED_LIST5 | UNORDERED_LIST6 | NONE,
                    {'-'},
                    {'>', UNORDERED_LINK1 | UNORDERED_LINK2 | UNORDERED_LINK3 | UNORDERED_LINK4 |
                              UNORDERED_LINK5 | UNORDERED_LINK6 | NONE}) != NONE)
            {
                return true;
            }
            // If we end up failing to parse an unordered modifier not all
            // hope is lost. We also have a weak paragraph delimiter which
            // looks like this:
            // ---
            // Detached modifiers are valid only if there is at least one
            // bit of whitespace after themselves. A delimiting modifier
            // strictly does not allow whitespace after itself. That's how
            // we differentiate between the two. m_ParsedChars is
            // incremented every time `check_detached` successfully parses a
            // character. We can use this to our advantage! The parser will
            // encounter 3 consecutive '-' chars and will parse all the way
            // up until the end. It will then try to return UNORDERED_LIST3
            // but will fail because there won't be any whitespace after the
            // 3 chars. It will then return NONE. Even though it may have
            // failed the m_ParsedChars value has still been modified! If
            // m_ParsedChars is 3 then that means we have parsed '---' and
            // hence we return a WEAK_PARAGRAPH_DELIMITER. This check is
            // even further enforced by checking if the next char is a
            // newline, which makes sense considering the parser head:
            // ---
            //   ^ will be here, and lexer->lookahead will return '\n'
            else if (lexer->lookahead == '\n' && m_ParsedChars >= 3)
            {
                lexer->result_symbol = m_LastToken = WEAK_PARAGRAPH_DELIMITER;
                return true;
            }

            if (check_detached(lexer,
                               ORDERED_LIST1 | ORDERED_LIST2 | ORDERED_LIST3 | ORDERED_LIST4 |
                                   ORDERED_LIST5 | ORDERED_LIST6 | NONE,
                               {'~'},
                               {'>', ORDERED_LINK1 | ORDERED_LINK2 | ORDERED_LINK3 | ORDERED_LINK4 |
                                         ORDERED_LINK5 | ORDERED_LINK6 | NONE}) != NONE)
                return true;

            if (check_detached(lexer, MARKER | NONE, {'|'}) != NONE)
                return true;

            if (check_detached(lexer, SINGLE_DEFINITION | MULTI_DEFINITION | NONE, {'$'}) != NONE)
                return true;
            else if (lexer->lookahead == '\n' && m_ParsedChars == 2)
            {
                lexer->result_symbol = MULTI_DEFINITION_SUFFIX;
                return true;
            }

            if (check_detached(lexer, SINGLE_FOOTNOTE | MULTI_FOOTNOTE, {'^'}) != NONE)
                return true;
            else if (lexer->lookahead == '\n' && m_ParsedChars == 2)
            {
                lexer->result_symbol = MULTI_FOOTNOTE_SUFFIX;
                return true;
            }

            if (check_detached(lexer, INSERTION, {'='}) != NONE)
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

            if (check_detached(lexer, NONE, {'_'}) != NONE)
                return true;
            else if (lexer->lookahead == '\n' && m_ParsedChars >= 3)
            {
                lexer->result_symbol = m_LastToken = HORIZONTAL_LINE;
                return true;
            }
        }

        if (lexer->lookahead == '~' && (m_LastToken == WORD || m_LastToken == CAPITALIZED_WORD))
        {
            advance(lexer);
            lexer->mark_end(lexer);

            if (lexer->lookahead == '\n')
            {
                advance(lexer);

                if (lexer->eof(lexer))
                    return false;

                lexer->result_symbol = m_LastToken = TRAILING_MODIFIER;
                return true;
            }

            return parse_text(lexer);
        }
        else if (lexer->lookahead == '\\')  // Check for an escape seqence (e.g. "\*")
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
        // If we are not in a tag and we have a square bracket opening then try
        // matching either a todo item or beginning of a list
        else if (m_LastToken >= UNORDERED_LIST1 && m_LastToken <= UNORDERED_LIST6 &&
                 lexer->lookahead == '[')
        {
            advance(lexer);

            lexer->mark_end(lexer);

            switch (lexer->lookahead)
            {
            // We're dealing with an undone item ([ ])
            case ' ':
                lexer->result_symbol = m_LastToken = TODO_ITEM_UNDONE;
                break;
            // We're dealing with a pending item ([-])
            case '-':
                lexer->result_symbol = m_LastToken = TODO_ITEM_PENDING;
                break;
            // We're dealing with a done item ([x])
            case 'x':
                lexer->result_symbol = m_LastToken = TODO_ITEM_DONE;
                break;
            // We're dealing with an item that has been put on hold ([=])
            case '=':
                lexer->result_symbol = m_LastToken = TODO_ITEM_ON_HOLD;
                break;
            // We're dealing with an item that has been cancelled ([_])
            case '_':
                lexer->result_symbol = m_LastToken = TODO_ITEM_CANCELLED;
                break;
            // We're dealing with an item that is urgent ([!])
            case '!':
                lexer->result_symbol = m_LastToken = TODO_ITEM_URGENT;
                break;
            // We're dealing with an item that is recurring ([+])
            case '+':
                lexer->result_symbol = m_LastToken = TODO_ITEM_RECURRING;
                break;
            // We're dealing with an item that needs further elaboration ([?])
            case '?':
                lexer->result_symbol = m_LastToken = TODO_ITEM_UNCERTAIN;
                break;
            default:
                advance(lexer);
                return false;
            }

            advance(lexer);  // Move past the matched element (*/x/ )

            if (lexer->lookahead == ']')
            {
                advance(lexer);
                lexer->mark_end(lexer);
                return true;
            }
            else
                return false;
        }
        else if (lexer->lookahead == '{' || (m_LastToken >= LINK_BEGIN && m_LastToken < LINK_END))
            return parse_link(lexer);
        else if ((lexer->lookahead == '[' && m_LastToken == LINK_END) ||
                 (m_LastToken >= LINK_TEXT_BEGIN && m_LastToken < LINK_TEXT_END))
            return parse_link_text(lexer);
        else if ((lexer->lookahead == '[' && m_LastToken != LINK_END) ||
                 (m_LastToken >= ANCHOR_DECLARATION_BEGIN && m_LastToken < ANCHOR_DECLARATION_END))
            return parse_anchor(lexer);

        // If we are not in a ranged tag then we should also check for potential
        // attached modifiers, like *this*.
        if (check_attached(lexer) != NONE)
            return true;

        // Match paragraphs
        return parse_text(lexer);
    }

    size_t& get_tag_level() noexcept { return m_TagLevel; }
    TokenType& get_last_token() noexcept { return m_LastToken; }
    int32_t& get_current_char() noexcept { return m_Current; }

   private:
    // Skips the next character without including it in the final result
    void skip(TSLexer* lexer)
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        return lexer->advance(lexer, true);
    }

    // Advances the lexer forward. The char that was advanced will be returned
    // in the final result
    void advance(TSLexer* lexer)
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        return lexer->advance(lexer, false);
    }

    // An alternate implementation for `check_detached` in case you don't have a
    // bunch of chained results
    template <size_t Size = 1>
    inline TokenType check_detached(TSLexer* lexer,
                                    TokenType result,
                                    const std::array<int32_t, Size>& expected,
                                    std::pair<char, TokenType> terminate_at = {0, NONE})
    {
        return check_detached(lexer, result | NONE, expected,
                              {terminate_at.first, terminate_at.second | NONE});
    }

    /*
     * Checks for the existence of a detached modifier
     * @param lexer - a pointer to the treesitter lexer
     * @param results - a list of potential results repending on the amount of
     * consecutive matches found
     * @param expected - a list of expected modifiers to appear in the sequence
     */
    template <size_t Size = 1>
    [[nodiscard]] TokenType check_detached(TSLexer* lexer,
                                           const std::vector<TokenType>& results,
                                           const std::array<int32_t, Size>& expected,
                                           std::pair<char, std::vector<TokenType>> terminate_at = {
                                               0, NONE | NONE})
    {
        static_assert(Size > 0, "check_detached Size template must be greater than 0");

        size_t i = m_ParsedChars = 0;

        // Loop as long as the next character is a valid detached modifier
        for (auto detached_modifier = std::find(m_DetachedModifiers.begin(),
                                                m_DetachedModifiers.end(), lexer->lookahead);
             detached_modifier != m_DetachedModifiers.end();
             detached_modifier = std::find(m_DetachedModifiers.begin(), m_DetachedModifiers.end(),
                                           lexer->lookahead),
                  i++, m_ParsedChars++)
        {
            // If we've specified a termination character and we match then the
            // token lexing prematurely
            if (terminate_at.first != 0 && lexer->lookahead == terminate_at.first)
            {
                advance(lexer);

                // Skip other potential whitespace
                while (lexer->lookahead && (lexer->lookahead == ' ' || lexer->lookahead == '\t'))
                    advance(lexer);

                TokenType result =
                    terminate_at.second[clamp(i, size_t {}, terminate_at.second.size()) - 1];

                lexer->result_symbol = m_LastToken = result;

                return result;
            }

            // If the next character is not one we expect then break
            // We use clamp() here to prevent overflow and to make the last
            // element of the expected array the fallback
            if (lexer->lookahead != expected[clamp(i, size_t {}, Size - 1)])
                break;

            advance(lexer);

            // If the next character is whitespace (which is the distinguishing
            // factor between an attached/detached modifier)
            if (std::iswspace(lexer->lookahead) && (lexer->lookahead != '\n'))
            {
                // Retrieve the correct result from the list of provided results
                // depending on how many characters were matched. If we've
                // exceeded the number of results then the clamp function will
                // fall back to the last element
                TokenType result = results[clamp(i, size_t {}, results.size() - 1)];

                // Skip any other potential whitespace
                while (lexer->lookahead && (lexer->lookahead == ' ' || lexer->lookahead == '\t'))
                    advance(lexer);

                lexer->result_symbol = m_LastToken = result;

                return result;
            }
        }

        // If we've only parsed one character and instantly failed then we might
        // be dealing with an attached modifier!
        if (m_ParsedChars == 1)
        {
            auto found_attached_modifier = m_AttachedModifiers.find(m_Current);

            if (found_attached_modifier != m_AttachedModifiers.end())
            {
                lexer->result_symbol = m_LastToken = found_attached_modifier->second;
                return m_LastToken;
            }
        }

        return NONE;
    }

    /*
     * Checks for the existence of an attached modifier
     * @param lexer - a pointer to the treesitter lexer
     * @param behind - if true the parser will use m_Current instead of
     * lexer->lookahead as its main lookahead source
     */
    TokenType check_attached(TSLexer* lexer)
    {
        if (lexer->lookahead == ':')
        {
            if (m_Current != '|' && m_AttachedModifiers.find(m_Current) == m_AttachedModifiers.end())
            {
                advance(lexer);

                if (lexer->lookahead != '|' && m_AttachedModifiers.find(lexer->lookahead) == m_AttachedModifiers.end())
                    return NONE;
            }
            else
                advance(lexer);

            if (std::iswspace(lexer->lookahead))
                return NONE;

            lexer->result_symbol = m_LastToken = LINK_MODIFIER;
            return m_LastToken;
        }
        else if ((!std::iswspace(m_Current) || !m_Current) && lexer->lookahead == '|')
        {
            advance(lexer);

            if (std::iswpunct(lexer->lookahead) || std::iswspace(lexer->lookahead) || !lexer->lookahead)
            {
                lexer->result_symbol = m_LastToken = MARKUP_CLOSE;
                return m_LastToken;
            }

            return NONE;
        }

        auto found_attached_modifier = m_AttachedModifiers.find(lexer->lookahead);

        if (found_attached_modifier == m_AttachedModifiers.end())
            return NONE;

        if ((std::iswspace(m_Current) || std::ispunct(m_Current) || !m_Current))
        {
            advance(lexer);

            if (lexer->lookahead == '|')
            {
                advance(lexer);

                if (!std::iswspace(lexer->lookahead))
                {
                    lexer->result_symbol = m_LastToken = static_cast<TokenType>(found_attached_modifier->second + (VARIABLE_OPEN - BOLD_OPEN) + 1);
                    return m_LastToken;
                }
            }
            if (!std::iswspace(lexer->lookahead))
            {
                lexer->result_symbol = m_LastToken = found_attached_modifier->second;
                return m_LastToken;
            }
        }

        advance(lexer);

        return NONE;
    }

    /*
     * Attempts to parse a link {* Like this}
     */
    bool parse_link(TSLexer* lexer)
    {
        if (lexer->lookahead == '{')
        {
            advance(lexer);
            lexer->result_symbol = m_LastToken = LINK_BEGIN;
            return true;
        }

        size_t count = 0;

        switch (m_LastToken)
        {
        case LINK_BEGIN:
            if (lexer->lookahead == ':')
            {
                advance(lexer);
                lexer->result_symbol = m_LastToken = LINK_FILE_BEGIN;
                return true;
            }
        case LINK_FILE_END:
            switch (lexer->lookahead)
            {
            case '}':
                advance(lexer);

                lexer->result_symbol = m_LastToken = LINK_END;
                return true;
            case '*':
                advance(lexer);

                while (lexer->lookahead == '*')
                {
                    ++count;
                    advance(lexer);
                }

                lexer->result_symbol = m_LastToken =
                    static_cast<TokenType>(LINK_LOCATION_HEADING1 + clamp(count, 0ull, 5ull));
                advance(lexer);

                return std::iswspace(m_Current);
            case '#':
                lexer->result_symbol = m_LastToken = LINK_LOCATION_GENERIC;
                break;
            case '$':
                lexer->result_symbol = m_LastToken = LINK_LOCATION_DEFINITION;
                break;
            case '@':
                if (m_LastToken == LINK_FILE_END)
                    return false;

                lexer->result_symbol = m_LastToken = LINK_LOCATION_EXTERNAL_FILE;
                break;
            case '|':
                lexer->result_symbol = m_LastToken = LINK_LOCATION_MARKER;
                break;
            case '^':
                lexer->result_symbol = m_LastToken = LINK_LOCATION_FOOTNOTE;
                break;
            default:
                if (m_LastToken == LINK_FILE_END)
                    return false;

                lexer->result_symbol = m_LastToken = LINK_LOCATION_URL;
                return true;
            }

            advance(lexer);

            if (!std::iswspace(lexer->lookahead))
                return false;

            advance(lexer);
            return true;
        case LINK_FILE_BEGIN:
            while (lexer->lookahead)
            {
                if (lexer->lookahead == ':' && m_Current != '\\')
                    break;

                advance(lexer);
            }

            lexer->result_symbol = m_LastToken = LINK_FILE_TEXT;
            return true;
        case LINK_FILE_TEXT:
            if (lexer->lookahead == ':')
            {
                advance(lexer);
                lexer->result_symbol = m_LastToken = LINK_FILE_END;
                return true;
            }

            return false;
        case LINK_LOCATION_TEXT:
            if (lexer->lookahead == '}')
            {
                advance(lexer);
                lexer->result_symbol = m_LastToken = LINK_END;
                return true;
            }
        default:
            if (m_LastToken >= LINK_LOCATION_GENERIC && m_LastToken <= LINK_LOCATION_FOOTNOTE)
            {
                while (lexer->lookahead)
                {
                    if (lexer->lookahead == '}' && m_Current != '\\')
                        break;

                    if (lexer->lookahead == '\n')
                    {
                        advance(lexer);

                        if (!lexer->lookahead || lexer->lookahead == '\n')
                            break;
                    }

                    advance(lexer);
                }

                lexer->result_symbol = m_LastToken = LINK_LOCATION_TEXT;
                return true;
            }
        }

        return false;
    }

    bool parse_link_text(TSLexer* lexer)
    {
        switch (m_LastToken)
        {
        case LINK_TEXT_BEGIN:
            while (lexer->lookahead)
            {
                if (lexer->lookahead == ']' && m_Current != '\\')
                    break;

                advance(lexer);
            }

            lexer->result_symbol = m_LastToken = LINK_TEXT;
            return true;
        case LINK_TEXT:
            if (lexer->lookahead == ']')
                lexer->result_symbol = m_LastToken = LINK_TEXT_END;

            break;
        case LINK_END:
            if (lexer->lookahead == '[')
                lexer->result_symbol = m_LastToken = LINK_TEXT_BEGIN;

            break;
        default:
            advance(lexer);
            return false;
        }

        advance(lexer);
        return true;
    }

    bool parse_anchor(TSLexer* lexer)
    {
        if (lexer->lookahead == '[')
        {
            advance(lexer);
            lexer->result_symbol = m_LastToken = ANCHOR_DECLARATION_BEGIN;
            return true;
        }

        switch (m_LastToken)
        {
        case ANCHOR_DECLARATION_BEGIN:
            while (lexer->lookahead)
            {
                if (lexer->lookahead == ']' && m_Current != '\\')
                    break;

                advance(lexer);
            }

            lexer->result_symbol = m_LastToken = ANCHOR_DECLARATION_TEXT;
            return true;
        case ANCHOR_DECLARATION_TEXT:
            if (lexer->lookahead == ']')
                lexer->result_symbol = m_LastToken = ANCHOR_DECLARATION_END;

            advance(lexer);
            return true;
        default:
            advance(lexer);
            return false;
        }

        return false;
    }

    /*
     * Simply parses any word (segment containing consecutive non-whitespace
     * characters). If in a tag (m_TagLevel) parse_text parses
     * till a newline is encountered
     */
    bool parse_text(TSLexer* lexer)
    {
        if (m_TagLevel)
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

        const TokenType resulting_symbol =
            (bool)std::iswupper(lexer->lookahead) ? CAPITALIZED_WORD : WORD;

        do
        {
            if (lexer->lookahead == ':')
                break;
            else if (((lexer->lookahead == '~' || lexer->lookahead == '|') && !std::iswspace(m_Current)) ||
                (m_AttachedModifiers.find(lexer->lookahead) != m_AttachedModifiers.end()))
                break;
            else
                advance(lexer);
        } while (lexer->lookahead && !std::iswspace(lexer->lookahead) && lexer->lookahead != '\\');

        lexer->result_symbol = m_LastToken = resulting_symbol;
        return true;
    }

    template <typename T, typename Comp>
    T clamp(T value, Comp min, Comp max)
    {
        return value < min ? min : (value > max ? max : value);
    }

   private:
    // Stores the current char rather than the next char
    int32_t m_Previous = 0, m_Current = 0;

    size_t m_TagLevel = 0;

    // The last matched token type (used to detect things like todo items
    // which require an unordered list prefix beforehand)
    TokenType m_LastToken = NONE;

    // Used for lookback
    size_t m_ParsedChars = 0;

   private:
    const std::array<int32_t, 9> m_DetachedModifiers = {'*', '-', '>', '|', '=',
                                                        '~', '$', '_', '^'};
    const std::unordered_map<char, TokenType> m_AttachedModifiers = {
        {'*', BOLD_OPEN},
        {'-', STRIKETHROUGH_OPEN},
        {'_', UNDERLINE_OPEN},
        {'/', ITALIC_OPEN},
        {'~', SPOILER_OPEN},
        {'^', SUPERSCRIPT_OPEN},
        {',', SUBSCRIPT_OPEN},
        {'`', VERBATIM_OPEN},
        {'+', INLINE_COMMENT_OPEN},
        {'$', INLINE_MATH_OPEN},
        {'=', VARIABLE_OPEN},
    };
};

extern "C"
{
    void* tree_sitter_norg_external_scanner_create() { return new Scanner(); }

    void tree_sitter_norg_external_scanner_destroy(void* payload)
    {
        delete static_cast<Scanner*>(payload);
    }

    bool tree_sitter_norg_external_scanner_scan(void* payload,
                                                TSLexer* lexer,
                                                const bool* valid_symbols)
    {
        return static_cast<Scanner*>(payload)->scan(lexer, valid_symbols);
    }

    unsigned tree_sitter_norg_external_scanner_serialize(void* payload, char* buffer)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);

        const auto& tag_level = scanner->get_tag_level();
        const auto& last_token = scanner->get_last_token();
        const auto& current = scanner->get_current_char();

        if (3 >= TREE_SITTER_SERIALIZATION_BUFFER_SIZE)
            return 0;

        buffer[0] = last_token;
        buffer[1] = tag_level;
        // TODO: We're storing an int32_t inside of a `char` here, meaning
        // loss of information. Should we divide the `current` value
        // into 4 pieces and store those inside this buffer?
        buffer[2] = current;

        return 3;
    }

    void tree_sitter_norg_external_scanner_deserialize(void* payload,
                                                       const char* buffer,
                                                       unsigned length)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);

        auto& tag_level = scanner->get_tag_level();
        auto& last_token = scanner->get_last_token();
        auto& current = scanner->get_current_char();

        if (length == 3)
        {
            last_token = (TokenType)buffer[0];
            tag_level = (size_t)buffer[1];
            current = (int32_t)buffer[2];
        }
        else
            tag_level = 0;
    }
}
