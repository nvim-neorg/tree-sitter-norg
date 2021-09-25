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

    PARAGRAPH_SEGMENT,
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
    RANGED_TAG_NAME,
    RANGED_TAG_END,

    CARRYOVER_TAG,

    SINGLE_DEFINITION,
    MULTI_DEFINITION,
    MULTI_DEFINITION_SUFFIX,
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
        if (!lexer->lookahead || lexer->eof(lexer))
        {
            advance(lexer);
            return false;
        }

        // Check for an escape seqence (e.g. "\*")
        if (lexer->lookahead == '\\')
        {
            advance(lexer);

            if (m_TagStack.empty() && lexer->lookahead)
            {
                lexer->result_symbol = ESCAPE_SEQUENCE;
                return true;
            }
            else
                return false;
        }

        // If we are not in a tag and we have a square bracket opening then try matching
        // either a todo item or beginning of a list
        if (m_TagStack.empty() && lexer->lookahead == '[')
        {
            advance(lexer);

            if (lexer->lookahead == ']')
            {
                lexer->result_symbol = PARAGRAPH_SEGMENT;
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
                    lexer->result_symbol = TODO_ITEM_UNDONE;
                    return true;
                // We're dealing with a pending item ([*])
                case '*':
                    lexer->result_symbol = TODO_ITEM_PENDING;
                    break;
                // We're dealing with a done item ([x])
                case 'x':
                    lexer->result_symbol = TODO_ITEM_DONE;
                    break;
                case '\0':
                    advance(lexer);
                    return false;
            }

            // Move past the closing ] character
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
        else if (m_TagStack.size() == 0 && lexer->lookahead == '(')
            return check_link(lexer);
        // Otherwise just check whether or not we're dealing with a newline and return STANDALONE_BREAK if we are
        else if (lexer->lookahead == '\n')
        {
            advance(lexer);
            lexer->result_symbol = PARAGRAPH_BREAK;
            return true;
        }

        // If we're at the beginning of a line check for all detached modifiers
        if (lexer->get_column(lexer) == 0)
        {
            m_IndentationLevel = 0;

            // Skip all leading whitespace
            while (lexer->lookahead == ' ' || lexer->lookahead == '\t')
            {
                if (lexer->lookahead == '\t')
                    m_IndentationLevel += 4;
                else
                    ++m_IndentationLevel;

                skip(lexer);
            }

            if (lexer->lookahead == '@')
            {
                advance(lexer);

                lexer->mark_end(lexer);

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

                                if (std::iswspace(lexer->lookahead) && m_TagStack.size() > 0)
                                {
                                    if (m_IndentationLevel != m_TagStack.back())
                                    {
                                        lexer->result_symbol = m_LastToken = PARAGRAPH_SEGMENT;
                                        return true;
                                    }
                                    else
                                    {
                                        lexer->result_symbol = m_LastToken = RANGED_TAG_END;

                                        m_TagStack.pop_back();
                                        return true;
                                    }
                                }

                                lexer->result_symbol = m_LastToken = PARAGRAPH_SEGMENT;
                                return true;
                            }
                        }
                    }
                }

                lexer->result_symbol = m_LastToken = RANGED_TAG;
                m_TagStack.push_back(m_IndentationLevel);
                return true;
            }
            else if (lexer->lookahead == '$')
            {
                advance(lexer);
                lexer->result_symbol = CARRYOVER_TAG;
                return true;
            }

            if (m_TagStack.size() == 0)
            {
                if (check_detached(lexer, HEADING1 | HEADING2 | HEADING3 | HEADING4 | HEADING5 | HEADING6, { '*' }) != NONE)
                    return true;

                if (check_detached(lexer, QUOTE1 | QUOTE2 | QUOTE3 | QUOTE4 | QUOTE5 | QUOTE6 | NONE, { '>' }) != NONE)
                    return true;

                if (check_detached(lexer, UNORDERED_LIST1 | UNORDERED_LIST2 | UNORDERED_LIST3 | UNORDERED_LIST4 | UNORDERED_LIST5 | UNORDERED_LIST6 | NONE, { '-' },
                            { '>', UNORDERED_LINK1 | UNORDERED_LINK2 | UNORDERED_LINK3 | UNORDERED_LINK4 | UNORDERED_LINK5 | UNORDERED_LINK6 | NONE }) != NONE)
                {
                    return true;
                }
                else if (lexer->lookahead == '\n' && m_ParsedChars >= 3)
                {
                    lexer->result_symbol = WEAK_PARAGRAPH_DELIMITER;
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
                else if (lexer->lookahead == '\n')
                {
                    if (m_ParsedChars >= 3)
                    {
                        lexer->result_symbol = STRONG_PARAGRAPH_DELIMITER;
                        return true;
                    }
                    else
                    {
                        advance(lexer);
                        lexer->result_symbol = PARAGRAPH_SEGMENT;
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

        // Match paragraphs
        if (valid_symbols[PARAGRAPH_SEGMENT] && lexer->lookahead != '\n')
            return parse_text(lexer);

        return false;
    }

    std::vector<size_t>& get_tag_stack() noexcept { return m_TagStack; }
private:
    void skip(TSLexer* lexer)
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        return lexer->advance(lexer, true);
    }

    void advance(TSLexer* lexer)
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        return lexer->advance(lexer, false);
    }

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

                    lexer->result_symbol = result;

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

                    lexer->result_symbol = result;

                    m_LastToken = result;

                    return result;
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
                lexer->result_symbol = LINK_END_GENERIC;
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

        return true;
    }

    /*
     * Simply parses any line (also called a paragraph segment)
     */
    bool parse_text(TSLexer* lexer)
    {
        while (lexer->lookahead)
        {
            if (m_TagStack.size() > 0 && m_IndentationLevel < m_TagStack.back())
            {
                lexer->result_symbol = RANGED_TAG_END;
                return true;
            }

            // If we have an escape sequence in the middle of the paragraph then terminate the paragraph
            // to allow the escape sequence to get parsed
            if (lexer->lookahead == '\\')
            {
                lexer->mark_end(lexer);

                advance(lexer);

                if (!lexer->lookahead)
                    continue;

                m_LastToken = PARAGRAPH_SEGMENT;
                lexer->result_symbol = PARAGRAPH_SEGMENT;
                return true;
            }

            // Try and find an occurrence of a trailing modifier
            if (!std::iswspace(m_Current) && lexer->lookahead == '~')
            {
                advance(lexer);

                // If we've managed to find one then skip over the newline and continue parsing
                if (lexer->lookahead == '\n')
                    continue;
            }
            // A [ is a special symbol - it can both mean a todo item and a link
            // Halt the parsing of the paragraph segment if such a thing is encountered
            else if (m_TagStack.size() == 0 && (std::iswspace(m_Current) || std::iswpunct(m_Current)) && lexer->lookahead == '[')
            {
                m_LastToken = PARAGRAPH_SEGMENT;
                lexer->result_symbol = PARAGRAPH_SEGMENT;
                return true;
            }

            advance(lexer);

            if (lexer->lookahead == '\n')
                break;
        }

        // Mark the end of the token so any subsequent calls to advance() don't get
        // appended to the result
        lexer->mark_end(lexer);

        // If the next char is valid then advance again
        if (lexer->lookahead)
            advance(lexer);

        // If we haven't reached EOF then treat the last call to advance() as part of the result too.
        // We do this because otherwise the parser would read one char past EOF which would mess things up
        if (!lexer->eof(lexer))
            lexer->mark_end(lexer);

        m_LastToken = PARAGRAPH_SEGMENT;
        lexer->result_symbol = PARAGRAPH_SEGMENT;

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

    // Used for tags
    std::vector<size_t> m_TagStack;

private:
    const std::array<int32_t, 8> s_DetachedModifiers = { '*', '-', '>', '|', '=', '~', ':', '_' };
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
        auto& tag_stack = static_cast<Scanner*>(payload)->get_tag_stack();

        if (tag_stack.size() >= TREE_SITTER_SERIALIZATION_BUFFER_SIZE)
            return 0;

        buffer = std::copy(tag_stack.begin(), tag_stack.end(), buffer);

        return tag_stack.size();
    }

    void tree_sitter_norg_external_scanner_deserialize(void* payload, const char* buffer, unsigned length)
    {
        auto& tag_stack = static_cast<Scanner*>(payload)->get_tag_stack();

        tag_stack.clear();
        tag_stack.resize(length);

        std::copy_n(buffer, length, tag_stack.begin());
    }
}
