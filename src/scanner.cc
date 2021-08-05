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

	QUOTE,
	UNORDERED_LIST,
    MARKER,

    PARAGRAPH_DELIMITER,
};

// Operator overloads for TokenTypes (allows for their chaining)
namespace
{
    std::vector<TokenType> operator|(TokenType lhs, TokenType rhs)
    {
        return std::vector<TokenType> { lhs, rhs };
    }

    decltype(auto) operator|(std::vector<TokenType>&& lhs, TokenType rhs)
    {
        lhs.push_back(rhs);
        return std::move(lhs);
    }
}

namespace utils
{
    // Generic clamp function, I'm aware of std::clamp but I haven't been bothered to set my compile_commands.json to use C++17/C++20 yet lol
    template <typename T>
    T clamp(const T& value, const T& min, const T& max)
    {
        return (value < min ? min : (value > max ? max : value));
    }
}

class Scanner
{
public:

	void skip(TSLexer* lexer)
	{
	    m_Current = lexer->lookahead;
		return lexer->advance(lexer, true);
	}

	void advance(TSLexer* lexer)
	{
	    m_Current = lexer->lookahead;
		return lexer->advance(lexer, false);
	}

    template <size_t Size = 1>
    inline TokenType check_detached(TSLexer* lexer, TokenType result, const std::array<unsigned char, Size>& expected)
    {
        return check_detached(lexer, std::vector<TokenType>(1, result), expected);
    }

    /*
     * Checks for the existence of a detached modifier
     * @param lexer - a pointer to the treesitter lexer
     * @param results - a list of potential results repending on the amount of consecutive matches found
     * @param expected - a list of expected modifiers to appear in the sequence
     */
    template <size_t Size = 1>
    [[nodiscard("You want to check whether we managed to match a detached token, not just let a function aimlessly run doofus")]]
    TokenType check_detached(TSLexer* lexer, const std::vector<TokenType>& results, const std::array<unsigned char, Size>& expected)
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
            // If the next character is not one we expect then break
            // We use clamp() here to prevent overflow and to make the last element of the expected array the fallback
            if (lexer->lookahead != expected[utils::clamp(i, 0UL, Size - 1)])
                break;

            advance(lexer);

            // If the next character is whitespace (which is the distinguishing factor between an attached/detached modifier)
            if (std::iswspace(lexer->lookahead))
            {
                // Make sure that the amount of characters we read is greater or equal to that of the expected chars,
                // otherwise we must have matched something incorrectly
                if (i < Size - 1)
                    break;

                // Retrieve the correct result from the list of provided results depending on how many characters were matched.
                // If we've exceeded the number of results then the clamp function will fall back to the last element
                TokenType result = results[utils::clamp(i, 0UL, results.size() - 1)];

                // Skip any other potential whitespace
                while (lexer->lookahead && std::iswspace(lexer->lookahead))
                    advance(lexer);

                lexer->result_symbol = result;
                lexer->mark_end(lexer);

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
            lexer->mark_end(lexer);

            return result;
        }

        return NONE;
    }

	bool scan(TSLexer* lexer, const bool* valid_symbols)
	{
	    // Are we at the end of file? If so, bail
	    if (lexer->eof(lexer))
	        return false;

        // Check for an escape seqence (e.g. "\*")
        if (lexer->lookahead == '\\')
        {
            advance(lexer);

            lexer->result_symbol = ESCAPE_SEQUENCE;
            lexer->mark_end(lexer);
            return true;
        }

        if (check_delimiting(lexer, '=', PARAGRAPH_DELIMITER) != NONE)
            return true;

        if (check_detached(lexer, HEADING1 | HEADING2 | HEADING3 | HEADING4 | HEADING5 | HEADING6, { '*' }) != NONE)
            return true;

        if (check_detached(lexer, QUOTE | NONE, { '>' }) != NONE)
            return true;

        if (check_detached(lexer, UNORDERED_LIST | NONE, { '-' }) != NONE)
            return true;

        if (check_detached(lexer, MARKER | NONE, { '|' }) != NONE)
            return true;

        // Match paragraphs
        if (valid_symbols[PARAGRAPH_SEGMENT] && lexer->lookahead != '\n')
        {
            while (lexer->lookahead && lexer->lookahead != '\n')
                advance(lexer);

            skip(lexer);

            lexer->result_symbol = PARAGRAPH_SEGMENT;
            lexer->mark_end(lexer);

            return true;
        }

        return false;
	}
private:
	unsigned char m_Current = 0;
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
