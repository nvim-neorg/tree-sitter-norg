#include "tree_sitter/parser.h"
#include <locale>
#include <iostream>
#include <regex>

enum TokenType
{
	BOLD
};

class Scanner
{
public:

	void skip(TSLexer* lexer)
	{
		return lexer->advance(lexer, true);
	}

	void advance(TSLexer* lexer)
	{
		return lexer->advance(lexer, false);
	}

	bool scan(TSLexer* lexer, const bool* valid_symbols)
	{
		if (valid_symbols[BOLD])
		{
			if (std::iswpunct(lexer->lookahead) || std::iswspace(lexer->lookahead))
			{
				skip(lexer);
				if (lexer->lookahead == '*')
				{
					skip(lexer);
					if (!std::iswspace(lexer->lookahead) && lexer->lookahead != '*')
					{
						advance(lexer);
						if (lexer->lookahead == '*')
							return false;

						while (lexer->lookahead && lexer->lookahead != '\n')
						{
							advance(lexer);

							if (lexer->lookahead == '\\')
							{
								advance(lexer);
								advance(lexer);
								continue;
							}

							else if (lexer->lookahead == '*')
							{
								skip(lexer);
								if (std::iswspace(lexer->lookahead) || std::iswpunct(lexer->lookahead))
								{
									lexer->result_symbol = BOLD;
									lexer->mark_end(lexer);
									return true;
								}
							}
						}
					}
				}
			}
		}

		/* if (valid_symbols[PARAGRAPH])
		{
			lexer->result_symbol = PARAGRAPH;
			return true;
		} */
		return false;
	}
private:
	std::string m_State;
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
