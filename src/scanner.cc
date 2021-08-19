#include "tree_sitter/parser.h"
#include <algorithm>
#include <locale>
#include <iostream>
#include <regex>
#include <string>

enum TokenType
{
    NONE,

    PARAGRAPH_SEGMENT,
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

    MARKER,
    DRAWER,
    DRAWER_SUFFIX,
    TODO_ITEM,
    UNORDERED_LINK,

    PARAGRAPH_DELIMITER,

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
    LINK_END_DRAWER_REFERENCE,
};

// Operator overloads for TokenTypes (allows for their chaining)
namespace
{
    std::vector<TokenType> operator|(TokenType lhs, TokenType rhs)
    {
        return std::vector<TokenType> { lhs, static_cast<TokenType>(rhs) };
    }

    decltype(auto) operator|(std::vector<TokenType>&& lhs, TokenType rhs)
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
        // Are we at the end of file? If so, bail
        if (lexer->eof(lexer))
            return false;

        lexer->result_symbol = NONE;

        if (!m_Whitespace && (lexer->lookahead == '\n' || !lexer->lookahead))
            m_Whitespace = false;
        else if (lexer->lookahead != '\n')
            m_Whitespace = true;

        // Check for an escape seqence (e.g. "\*")
        if (lexer->lookahead == '\\')
        {
            advance(lexer);

            lexer->result_symbol = ESCAPE_SEQUENCE;
            return true;
        }

        // If the last matched token was an unordered list check whether we are dealing with a todo item
        if (m_LastToken >= UNORDERED_LIST1 && m_LastToken <= UNORDERED_LIST6 && lexer->lookahead == '[')
        {
            advance(lexer);
            lexer->result_symbol = TODO_ITEM;
            return true;
        }
        // Otherwise make sure to check for the existence of links
        else if (lexer->lookahead == '[' || lexer->lookahead == '(')
            return check_link(lexer);

        // If we're at the beginning of a line check for all detached modifiers
        if (m_Whitespace)
        {
            if (check_detached(lexer, HEADING1 | HEADING2 | HEADING3 | HEADING4 | HEADING5 | HEADING6 | NONE, { '*' }) != NONE)
                return true;

            if (check_detached(lexer, QUOTE1 | QUOTE2 | QUOTE3 | QUOTE4 | QUOTE5 | QUOTE6 | NONE, { '>' }) != NONE)
                return true;

            // TODO: Add different unordered link levels
            if (check_detached(lexer, UNORDERED_LIST1 | UNORDERED_LIST2 | UNORDERED_LIST3 | UNORDERED_LIST4 | UNORDERED_LIST5 | UNORDERED_LIST6 | NONE, { '-' }, { '>', UNORDERED_LINK | NONE }) != NONE)
                    return true;

            if (check_detached(lexer, MARKER | DRAWER | NONE, { '|' }) != NONE)
                return true;

            if (m_Current == '|' && (lexer->lookahead == '\n' || !lexer->lookahead))
            {
                advance(lexer);
                lexer->result_symbol = DRAWER_SUFFIX;
                lexer->mark_end(lexer);
                return true;
            }

            if (check_delimiting(lexer, '=', PARAGRAPH_DELIMITER) != NONE)
                return true;
        }

        // Match paragraphs
        if (valid_symbols[PARAGRAPH_SEGMENT] && lexer->lookahead != '\n')
            return parse_text(lexer);

        return false;
    }
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
    inline TokenType check_detached(TSLexer* lexer, TokenType result, const std::array<unsigned char, Size>& expected, std::pair<char, TokenType> terminate_at = { 0, NONE })
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
    [[nodiscard("You want to check whether we managed to match a detached token, not just let a function aimlessly run doofus")]]
    TokenType check_detached(TSLexer* lexer, const std::vector<TokenType>& results, const std::array<unsigned char, Size>& expected, std::pair<char, std::vector<TokenType>> terminate_at = { 0, NONE | NONE })
    {
        static_assert(Size > 0, "check_detached Size template must be greater than 0");

        size_t i = 0;

        // Skip all trailing whitespace
        while (lexer->lookahead == ' ' || lexer->lookahead == '\t')
            skip(lexer);

        // Loop as long as the next character is a valid detached modifier
        for (auto detached_modifier = std::find(s_DetachedModifiers.begin(), s_DetachedModifiers.end(), lexer->lookahead);
                detached_modifier != s_DetachedModifiers.end();
                    detached_modifier = std::find(s_DetachedModifiers.begin(), s_DetachedModifiers.end(), lexer->lookahead), i++)
        {
            // If we've specified a termination character and we match then the token lexing prematurely
            if (lexer->lookahead == terminate_at.first)
            {
                advance(lexer);

                // Skip other potential whitespace
                while (lexer->lookahead && std::iswspace(lexer->lookahead))
                    advance(lexer);

                TokenType result = terminate_at.second[std::clamp(i, 0UL, terminate_at.second.size()) - 1];

                lexer->result_symbol = result;

                return result;
            }

            // If the next character is not one we expect then break
            // We use clamp() here to prevent overflow and to make the last element of the expected array the fallback
            if (lexer->lookahead != expected[std::clamp(i, 0UL, Size - 1)])
                break;

            advance(lexer);

            // If the next character is whitespace (which is the distinguishing factor between an attached/detached modifier)
            if (std::iswspace(lexer->lookahead) && (lexer->lookahead != '\n'))
            {
                // Retrieve the correct result from the list of provided results depending on how many characters were matched.
                // If we've exceeded the number of results then the clamp function will fall back to the last element
                TokenType result = results[std::clamp(i, 0UL, results.size() - 1)];

                // Skip any other potential whitespace
                while (lexer->lookahead && std::iswspace(lexer->lookahead))
                    advance(lexer);

                lexer->result_symbol = result;

                m_LastToken = result;

                return result;
            }
        }

        return NONE;
    }

    /* Checks for the existence of a delimiting modifier set
     * @param lexer - the treesitter lexer
     * @param c - the delimiting character to check for
     * @param result - the result to return if a match was successful
     * Delimiting modifiers sets consist of the same token being repeated 3 or more times.
     */
    TokenType check_delimiting(TSLexer* lexer, char c, TokenType result)
    {
        // If the next character is the one we're looking for continue
        if (lexer->lookahead == c)
        {
            advance(lexer);

            size_t count = 1;

            // Read the next token as long as it is the one we're looking for
            // We also keep track of the amount of tokens read
            while(lexer->lookahead == c)
            {
                ++count;
                advance(lexer);
            }

            // If we haven't read at least 3 tokens or if there's some trailing stuff after the last token then bail
            if (count < 3 || lexer->lookahead != '\n')
                return NONE;

            lexer->result_symbol = result;

            return result;
        }

        return NONE;
    }

    /*
     * Attempts to parse a link ([like](#this))
     */
    bool check_link(TSLexer* lexer)
    {
        // Are we dealing with the first segment of a link?
        if (lexer->lookahead == '[')
        {
            advance(lexer);

            // Until we don't reach the end keep parsing
            while (lexer->lookahead != ']')
            {
                advance(lexer);

                // If we've reached an EOL then bail
                if (lexer->lookahead == '\n' || !lexer->lookahead)
                    break;

                // Account for escaped chars
                if (m_Current == '\\')
                {
                    advance(lexer);
                    advance(lexer);
                    continue;
                }
            }

            // Make sure to capture the closing ] too!
            advance(lexer);

            m_LastToken = LINK_BEGIN;
            lexer->result_symbol = LINK_BEGIN;

            return true;
        }
        // This means we're dealing with a link location
        else if (lexer->lookahead == '(')
        {
            advance(lexer);

            if (lexer->lookahead == ':')
            {
                while (lexer->lookahead != '*' && lexer->lookahead != '#' && lexer->lookahead != '|' && lexer->lookahead != ')')
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
                while (lexer->lookahead == '*')
                {
                    advance(lexer);
                    ++heading_level;
                }

                // We use the clamp() here to make sure we don't overflow!
                lexer->result_symbol = m_LastToken = static_cast<TokenType>(LINK_END_HEADING1_REFERENCE + std::clamp(heading_level, 0UL, 5UL));
            }
            // We're dealing with one of two things: a marker reference or a drawer reference
            else if (m_Current == '|')
            {
                if (lexer->lookahead == '|')
                    lexer->result_symbol = m_LastToken = LINK_END_DRAWER_REFERENCE;
                else
                    lexer->result_symbol = m_LastToken = LINK_END_MARKER_REFERENCE;
            }
            // We're dealing with a generic (loose) link
            else if (m_Current == '#')
                lexer->result_symbol = m_LastToken = LINK_END_GENERIC;

            // Until we don't hit the end of the link location keep advancing
            while (lexer->lookahead != ')')
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

            return true;
        }

        return false;
    }
    
    /*
     * Simply parses any line (also called a paragraph segment)
     */
    bool parse_text(TSLexer* lexer)
    {
        while (lexer->lookahead)
        {
            // If we have an escape sequence in the middle of the paragraph then terminate the paragraph
            // to allow the escape sequence to get parsed
            if (lexer->lookahead == '\\')
            {
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
            else if (std::iswspace(m_Current) && lexer->lookahead == '[')
            {
                m_LastToken = PARAGRAPH_SEGMENT;
                lexer->result_symbol = PARAGRAPH_SEGMENT;
                return true;
            }

            advance(lexer);

            if (lexer->lookahead == '\n')
                break;
        }

        if (lexer->lookahead)
            advance(lexer);

        m_LastToken = PARAGRAPH_SEGMENT;
        lexer->result_symbol = PARAGRAPH_SEGMENT;

        return true;
    }

private:
    // Stores the current char rather than the next char
    unsigned char m_Previous = 0, m_Current = 0;

    // If true then we are at the beginning of a line (i.e. no non-whitespace chars have been encountered
    // since the beginning of the line)
    bool m_Whitespace = false;

    // The last matched token type (used to detect things like todo items
    // which require an unordered list prefix beforehand)
    TokenType m_LastToken = NONE;

    // Used for lookback
    std::string m_Buffer;

private:
    constexpr static const std::array<unsigned char, 4> s_DetachedModifiers = { '*', '-', '>', '|' };
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
          return 0;
    }

    void tree_sitter_norg_external_scanner_deserialize(void* payload, const char* buffer, unsigned length) {}
}
