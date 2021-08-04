#include "tree_sitter/parser.h"
#include <algorithm>
#include <locale>
#include <iostream>
#include <regex>
#include <string>

enum TokenType
{
    NONE,
	PARAGRAPH,
	PARAGRAPH_SEGMENT,
	HEADING1,
	HEADING2,
	HEADING3,
	HEADING4,
	HEADING5,
	HEADING6,
	QUOTE,
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

    template <size_t Size = 0>
    inline TokenType check_detached(TSLexer* lexer, TokenType result, const std::array<char, Size>& expected = {})
    {
        return check_detached(lexer, std::vector<TokenType>(1, result), expected);
    }

    /*
     * Checks for the existence of a detached modifier
     * @param lexer - a pointer to the treesitter lexer
     * @param results - a list of potential results repending on the amount of consecutive matches found
     * @param expected - a list of expected modifiers to appear in the sequence
     */
    template <size_t Size = 0>
    [[nodiscard("You want to check whether we managed to match a detached token, not just let a function aimlessly run doofus")]]
    TokenType check_detached(TSLexer* lexer, const std::vector<TokenType>& results, const std::array<char, Size>& expected = {})
    {
        size_t i = 0;

        for (auto detached_modifier = std::find(s_DetachedModifiers.begin(), s_DetachedModifiers.end(), lexer->lookahead);
                detached_modifier != s_DetachedModifiers.end();
                    detached_modifier = std::find(s_DetachedModifiers.begin(), s_DetachedModifiers.end(), lexer->lookahead), i++)
        {
            if (Size > 0 && lexer->lookahead != expected[utils::clamp(i, 0UL, Size - 1)])
                break;

            advance(lexer);

            if (std::iswspace(lexer->lookahead))
            {
                if (Size > 0 && i < Size - 1)
                    break;

                TokenType result = results[utils::clamp(i, 0UL, results.size() - 1)];

                lexer->result_symbol = result;
                lexer->mark_end(lexer);
                return result;
            }
        }

        return NONE;
    }

	bool scan(TSLexer* lexer, const bool* valid_symbols)
	{
        if (check_detached<1>(lexer, HEADING1 | HEADING2 | HEADING3 | HEADING4 | HEADING5 | HEADING6, { '*' }) != NONE)
            return true;

        if (check_detached<1>(lexer, QUOTE | NONE, { '>' }) != NONE)
            return true;

		return false;
	}
private:
	unsigned char m_Current = 0;
    constexpr static const std::array<unsigned char, 3> s_DetachedModifiers = { '*', '-', '>' };
};

extern "C"
{
	void* tree_sitter_norg_external_scanner_create()
	{
		return new Scanner();
	}

	void tree_sitter_norg_external_scanner_destroy(void* payload)
	{
  		delete (Scanner*)payload;
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
