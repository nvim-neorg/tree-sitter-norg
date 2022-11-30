#include <algorithm>
#include <array>
#include <bitset>
#include <cstring>
#include <cwctype>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <unordered_map>
#include "tree_sitter/parser.h"

using namespace std;

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

    DETACHED_MODIFIER_EXTENSION_BEGIN,
    MODIFIER_EXTENSION_DELIMITER,
    DETACHED_MODIFIER_EXTENSION_END,
    PRIORITY,
    TIMESTAMP,
    TODO_ITEM_UNDONE,
    TODO_ITEM_PENDING,
    TODO_ITEM_DONE,
    TODO_ITEM_ON_HOLD,
    TODO_ITEM_CANCELLED,
    TODO_ITEM_URGENT,
    TODO_ITEM_UNCERTAIN,
    TODO_ITEM_RECURRING,

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

    SINGLE_DEFINITION,
    MULTI_DEFINITION,
    MULTI_DEFINITION_SUFFIX,

    SINGLE_FOOTNOTE,
    MULTI_FOOTNOTE,
    MULTI_FOOTNOTE_SUFFIX,

    SINGLE_TABLE_CELL,
    MULTI_TABLE_CELL,
    MULTI_TABLE_CELL_SUFFIX,

    STRONG_PARAGRAPH_DELIMITER,
    WEAK_PARAGRAPH_DELIMITER,
    HORIZONTAL_LINE,

    LINK_DESCRIPTION_BEGIN,
    LINK_DESCRIPTION_END,
    LINK_LOCATION_BEGIN,
    LINK_LOCATION_END,
    LINK_FILE_BEGIN,
    LINK_FILE_END,
    LINK_FILE_TEXT,
    LINK_TARGET_URL,
    LINK_TARGET_LINE_NUMBER,
    LINK_TARGET_WIKI,
    LINK_TARGET_GENERIC,
    LINK_TARGET_EXTERNAL_FILE,
    LINK_TARGET_TIMESTAMP,
    LINK_TARGET_DEFINITION,
    LINK_TARGET_FOOTNOTE,
    LINK_TARGET_HEADING1,
    LINK_TARGET_HEADING2,
    LINK_TARGET_HEADING3,
    LINK_TARGET_HEADING4,
    LINK_TARGET_HEADING5,
    LINK_TARGET_HEADING6,

    TIMESTAMP_DATA,
    PRIORITY_DATA,

    TAG_DELIMITER,

    MACRO_TAG,
    MACRO_TAG_END,
    RANGED_TAG,
    RANGED_TAG_END,
    RANGED_VERBATIM_TAG,
    RANGED_VERBATIM_TAG_END,

    INFIRM_TAG,
    WEAK_CARRYOVER,
    STRONG_CARRYOVER,

    LINK_MODIFIER,
    INTERSECTING_MODIFIER,

    ATTACHED_MODIFIER_BEGIN,
    ATTACHED_MODIFIER_END,

    BOLD_OPEN,
    BOLD_CLOSE,

    ITALIC_OPEN,
    ITALIC_CLOSE,

    STRIKETHROUGH_OPEN,
    STRIKETHROUGH_CLOSE,

    UNDERLINE_OPEN,
    UNDERLINE_CLOSE,

    SPOILER_OPEN,
    SPOILER_CLOSE,

    SUPERSCRIPT_OPEN,
    SUPERSCRIPT_CLOSE,

    SUBSCRIPT_OPEN,
    SUBSCRIPT_CLOSE,

    VERBATIM_OPEN,
    VERBATIM_CLOSE,

    INLINE_COMMENT_OPEN,
    INLINE_COMMENT_CLOSE,

    INLINE_MATH_OPEN,
    INLINE_MATH_CLOSE,

    INLINE_MACRO_OPEN,
    INLINE_MACRO_CLOSE,

    FREE_FORM_MODIFIER_OPEN,
    FREE_FORM_MODIFIER_CLOSE,

    INLINE_LINK_TARGET_OPEN,
    INLINE_LINK_TARGET_CLOSE,

    SLIDE,
    INDENT_SEGMENT,
};

enum class TagType : char
{
    NONE = 1,

    ON_TAG,
    IN_TAG,

    ON_VERBATIM_TAG,
    IN_VERBATIM_TAG,
};

namespace
{

template <typename Type>
ostream& operator<<(ostream& stream, const vector<Type>& vec)
{
    stream << '{';
    for (auto& elem : vec)
        stream << elem << ",";
    stream << '}';
    return stream;
}

template <typename First, typename Second>
ostream& operator<<(ostream& stream, const pair<First, Second>& pair)
{
    return stream << '(' << (char)pair.first << ", " << (int)pair.second << ')';
}

}  // namespace

struct Scanner
{
    TSLexer* lexer;

    int32_t
    m_Previous = 0,  // previous char
    m_Current = 0;   // current char

    TagType m_TagContext = TagType::NONE;
    size_t m_TagLevel = 0;

    bool m_InLinkLocation = false;

    // The last matched token type (used to detect things like todo items
    // which require an unordered list prefix beforehand)
    TokenType m_LastToken = NONE;

    // Used for lookback
    size_t m_ParsedChars = 0;

    const array<int32_t, 12> m_DetachedModifiers = {
        '*',  // Headings
        '-',  // Unordered Lists
        '>',  // Quotes
        '%',  // Attributes
        '=',
        '~',  // Ordered Lists
        '$',  // Definitions
        '_',
        '^',  // Footnotes
        '&',
        '<',
        ':',  // Table cells
    };

    const unordered_map<int32_t, TokenType> m_DetachedModifierExtensions = {
        {'#', PRIORITY},
        {'@', TIMESTAMP},
        {' ', TODO_ITEM_UNDONE},
        {'-', TODO_ITEM_PENDING},
        {'x', TODO_ITEM_DONE},
        {'=', TODO_ITEM_ON_HOLD},
        {'_', TODO_ITEM_CANCELLED},
        {'!', TODO_ITEM_URGENT},
        {'+', TODO_ITEM_RECURRING},
        {'?', TODO_ITEM_UNCERTAIN},
    };

    const unordered_map<int32_t, TokenType> m_AttachedModifiers = {
        {'*', BOLD_OPEN},        {'/', ITALIC_OPEN},       {'-', STRIKETHROUGH_OPEN},
        {'_', UNDERLINE_OPEN},   {'!', SPOILER_OPEN},      {'`', VERBATIM_OPEN},
        {'^', SUPERSCRIPT_OPEN}, {',', SUBSCRIPT_OPEN},    {'%', INLINE_COMMENT_OPEN},
        {'$', INLINE_MATH_OPEN}, {'&', INLINE_MACRO_OPEN},
    };

    bitset<((INLINE_MACRO_OPEN - BOLD_OPEN) / 2) + 1> m_ActiveModifiers;

    bool scan(const bool* valid_symbols)
    {
        lexer->result_symbol = NONE;

        // Are we at the end of file? If so, bail.
        if (lexer->eof(lexer) || !lexer->lookahead)
        {
            reset_active_modifiers();
            return false;
        }

        if (m_LastToken == TRAILING_MODIFIER)
        {
            advance();
            return parse_text();
        }
        else if (is_newline(lexer->lookahead))
        {
            advance();

            lexer->result_symbol = m_LastToken = LINE_BREAK;

            if (lexer->eof(lexer) || !lexer->lookahead)
            {
                reset_active_modifiers();
                return true;
            }

            if (m_TagContext != TagType::NONE && ((char)m_TagContext % 2) == 0)
            {
                m_TagContext = (TagType)((char)m_TagContext + 1);
                return true;
            }

            if (is_newline(lexer->lookahead))
            {
                advance();
                lexer->result_symbol = m_LastToken = PARAGRAPH_BREAK;
                reset_active_modifiers();
            }

            return true;
        }

        // If we're at the beginning of a line check for all detached modifiers
        if (lexer->get_column(lexer) == 0)
        {
            // Skip all leading whitespace
            while (is_blank(lexer->lookahead))
                skip();

            // We are dealing with a ranged verbatim tag: @something
            if (lexer->lookahead == '@')
            {
                advance();

                // Mark the end of the token here
                // We do this because we only want the returned token to be part
                // of the `@` symbol, not the symbol + the name
                lexer->mark_end(lexer);

                // Check whether the tag is `@end`.
                if (token("end") && (iswspace(lexer->lookahead) || !lexer->lookahead))
                {
                    while (is_blank(lexer->lookahead))
                        advance();

                    if ((iswspace(lexer->lookahead) || !lexer->lookahead)
                        && m_TagContext == TagType::IN_VERBATIM_TAG)
                    {
                        lexer->result_symbol = m_LastToken = RANGED_VERBATIM_TAG_END;
                        m_TagContext = TagType::NONE;
                        return true;
                    }

                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }

                // This is a fallback. If the tag ends up not being `@end` then...
                if (m_LastToken == RANGED_VERBATIM_TAG || m_TagContext == TagType::IN_VERBATIM_TAG)
                {
                    // Ignore the char if we are already inside of a ranged tag.
                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }

                lexer->result_symbol = m_LastToken = RANGED_VERBATIM_TAG;
                m_TagContext = TagType::ON_VERBATIM_TAG;
                return true;
            }

            if (m_TagContext == TagType::IN_VERBATIM_TAG)
                return parse_text();

            // We are dealing with a macro tag (=something)
            if (lexer->lookahead == '=' && m_TagContext != TagType::IN_VERBATIM_TAG)
            {
                advance();

                // Mark the end of the token here
                // We do this because we only want the returned token to be part
                // of the `=` symbol, not the symbol + the name
                lexer->mark_end(lexer);

                // These sets of checks check whether the tag is `=end`
                if (token("end") && (iswspace(lexer->lookahead) || !lexer->lookahead))
                {
                    while (lexer->lookahead && iswspace(lexer->lookahead)
                           && !is_newline(lexer->lookahead))
                        advance();

                    if ((iswspace(lexer->lookahead) || !lexer->lookahead) && m_TagLevel)
                    {
                        lexer->result_symbol = m_LastToken = MACRO_TAG_END;
                        --m_TagLevel;
                        return true;
                    }

                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }
                else if (lexer->lookahead == '=')
                {
                    advance();
                    if (lexer->lookahead == '=')
                    {
                        // we are now three-characters in
                        do
                            advance();
                        while (lexer->lookahead == '=');

                        if (is_newline(lexer->lookahead))
                        {
                            // reset the marked end
                            lexer->mark_end(lexer);
                            advance();
                            lexer->result_symbol = m_LastToken = STRONG_PARAGRAPH_DELIMITER;
                            return true;
                        }
                        else
                        {
                            // reset the marked end
                            lexer->mark_end(lexer);
                            advance();
                            lexer->result_symbol = m_LastToken = WORD;
                            return true;
                        }
                    }
                    else
                    {
                        lexer->mark_end(lexer);
                        lexer->result_symbol = m_LastToken = WORD;
                        return true;
                    }
                }

                // This is a fallback. If the tag ends up not being `=end` then...
                if (m_LastToken == MACRO_TAG)
                {
                    // Ignore the char if we are already inside of a ranged tag.
                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }

                // or push back the indentation level and return.
                lexer->result_symbol = m_LastToken = MACRO_TAG;
                m_TagContext = TagType::ON_TAG;
                ++m_TagLevel;
                return true;
            }
            // We are dealing with a ranged tag (|something)
            else if (lexer->lookahead == '|' && m_TagContext != TagType::IN_VERBATIM_TAG)
            {
                advance();

                // Mark the end of the token here.
                // We do this because we only want the returned token to be part
                // of the `|` symbol, not the symbol + the name.
                lexer->mark_end(lexer);

                // These sets of checks check whether the tag is `|end`.
                if (token("end") && (iswspace(lexer->lookahead) || !lexer->lookahead))
                {
                    while (is_blank(lexer->lookahead))
                        advance();

                    if ((iswspace(lexer->lookahead) || !lexer->lookahead) && m_TagLevel)
                    {
                        lexer->result_symbol = m_LastToken = RANGED_TAG_END;
                        --m_TagLevel;
                        return true;
                    }

                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }

                // This is a fallback. If the tag ends up not being `|end` then...
                if (m_LastToken == RANGED_TAG)
                {
                    // Ignore the char if we are already inside of a ranged tag.
                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }

                // Or push back the indentation level and return.
                lexer->result_symbol = m_LastToken = RANGED_TAG;
                m_TagContext = TagType::ON_TAG;
                ++m_TagLevel;
                return true;
            }
            // we are dealing with a strong carryover (#something)
            else if (lexer->lookahead == '#' && m_TagContext != TagType::IN_VERBATIM_TAG)
            {
                advance();

                if (!lexer->lookahead || iswspace(lexer->lookahead))
                {
                    if (is_newline(lexer->lookahead))
                        lexer->result_symbol = m_LastToken = INDENT_SEGMENT;
                    else
                        lexer->result_symbol = m_LastToken = WORD;

                    return true;
                }

                lexer->result_symbol = m_LastToken = STRONG_CARRYOVER;
                return true;
            }
            // we are dealing with a weak carryover (+something)
            else if (lexer->lookahead == '+' && m_TagContext != TagType::IN_VERBATIM_TAG)
            {
                advance();
                if (lexer->lookahead != '+')
                {
                    lexer->result_symbol = m_LastToken = WEAK_CARRYOVER;
                    return true;
                }
            }
            // we are dealing with a infirm tag (.something)
            else if (lexer->lookahead == '.' && m_TagContext != TagType::IN_VERBATIM_TAG)
            {
                advance();
                if (lexer->lookahead != '.')
                {
                    lexer->result_symbol = m_LastToken = INFIRM_TAG;
                    return true;
                }
            }

            // The idea of the check_detached function is as follows:
            // We check for the '*' character and depending on how many we
            // encounter we return a different token If we encounter 1 '*'
            // char then return heading1, if we encounter two in a row then
            // return heading2, you get the idea. If we encounter more than
            // the given amount of fallbacks then the last fallback will
            // always be chosen. This means that if we have 7 consecutive
            // '*' chars then we will still fall back to the HEADING6 token
            // instead.
            if (check_detached({HEADING1, HEADING2, HEADING3,
                                HEADING4, HEADING5, HEADING6}, '*'))
                return true;

            // Check for the existence of quotes
            if (check_detached({QUOTE1, QUOTE2, QUOTE3, QUOTE4, QUOTE5, QUOTE6}, '>'))
                return true;

            // Check for the existence of an unordered list element.
            if (check_detached({UNORDERED_LIST1, UNORDERED_LIST2, UNORDERED_LIST3,
                                UNORDERED_LIST4, UNORDERED_LIST5, UNORDERED_LIST6}, '-'))
                return true;
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
            else if (is_newline(lexer->lookahead) && m_ParsedChars >= 3)
            {
                advance();
                lexer->result_symbol = m_LastToken = WEAK_PARAGRAPH_DELIMITER;
                return true;
            }

            if (check_detached({ORDERED_LIST1, ORDERED_LIST2, ORDERED_LIST3,
                                ORDERED_LIST4, ORDERED_LIST5, ORDERED_LIST6}, '~'))
                return true;
            else if (is_newline(lexer->lookahead) && m_ParsedChars == 1)
            {
                if (lexer->eof(lexer))
                {
                    reset_active_modifiers();
                    return false;
                }

                lexer->result_symbol = m_LastToken = TRAILING_MODIFIER;
                return true;
            }

            if (check_detached({SINGLE_DEFINITION, MULTI_DEFINITION, NONE}, '$'))
                return true;
            else if (is_newline(lexer->lookahead) && m_ParsedChars == 2)
            {
                advance();
                lexer->result_symbol = MULTI_DEFINITION_SUFFIX;
                return true;
            }

            if (check_detached({SINGLE_FOOTNOTE, MULTI_FOOTNOTE, NONE}, '^'))
                return true;
            else if (is_newline(lexer->lookahead) && m_ParsedChars == 2)
            {
                advance();
                lexer->result_symbol = MULTI_FOOTNOTE_SUFFIX;
                return true;
            }

            if (check_detached({SINGLE_TABLE_CELL, MULTI_TABLE_CELL, NONE}, ':'))
                return true;
            else if (is_newline(lexer->lookahead) && m_ParsedChars == 2)
            {
                advance();
                lexer->result_symbol = MULTI_TABLE_CELL_SUFFIX;
                return true;
            }

            if (check_detached({NONE, NONE}, '_'))
                return true;
            else if (is_newline(lexer->lookahead) && m_ParsedChars >= 3)
            {
                lexer->result_symbol = m_LastToken = HORIZONTAL_LINE;
                return true;
            }
        }

        switch (lexer->lookahead) {
        case '~':
            advance();
            lexer->mark_end(lexer);

            if (is_newline(lexer->lookahead))
            {
                advance();
                if (lexer->eof(lexer))
                {
                    reset_active_modifiers();
                    return false;
                }
                lexer->result_symbol = m_LastToken = TRAILING_MODIFIER;
                return true;
            }

            return parse_text();
        case '\\':  // Check for an escape seqence (e.g. "\*")
            advance();
            lexer->result_symbol = m_LastToken = ESCAPE_SEQUENCE;
            return true;
        }

        if (check_detached_mod_extension())
            return true;
        else if (((m_LastToken >= HEADING1 && m_LastToken <= MULTI_TABLE_CELL_SUFFIX)
                  || m_LastToken == DETACHED_MODIFIER_EXTENSION_END)
                 && lexer->lookahead == ':')
        {
            advance();
            bool is_indent_segment = false;

            if (lexer->lookahead == ':')
            {
                advance();
                is_indent_segment = true;
            }

            if (!is_newline(lexer->lookahead))
            {
                lexer->result_symbol = m_LastToken = WORD;
                return true;
            }

            // Move past the newline character as well
            advance();

            lexer->result_symbol = m_LastToken = (TokenType)(SLIDE + is_indent_segment);
            return true;
        }

        switch (lexer->lookahead) {
        case '<':
            advance();

            if (!iswspace(lexer->lookahead))
            {
                lexer->result_symbol = m_LastToken = INLINE_LINK_TARGET_OPEN;
                m_InLinkLocation = true;
                return true;
            }
            break;
        case '>':
            advance();

            if (!iswspace(m_Previous) && m_LastToken != LINK_LOCATION_BEGIN
                && m_LastToken != LINK_FILE_END)
            {
                lexer->result_symbol = m_LastToken = INLINE_LINK_TARGET_CLOSE;
                m_InLinkLocation = false;
                return true;
            }
            break;
        case '(':
            advance();

            if (!iswspace(lexer->lookahead) && m_LastToken != NONE
                && ((m_LastToken >= BOLD_OPEN && m_LastToken <= INLINE_MACRO_CLOSE
                     && (m_LastToken % 2) == (BOLD_CLOSE % 2))
                    || m_LastToken == LINK_DESCRIPTION_END
                    || m_LastToken == LINK_LOCATION_END
                    || m_LastToken == INLINE_LINK_TARGET_CLOSE))
            {
                lexer->result_symbol = m_LastToken = ATTACHED_MODIFIER_BEGIN;
                return true;
            }
            else
            {
                lexer->result_symbol = m_LastToken = WORD;
                return true;
            }
            break;
        case ')':
            advance();

            if (!iswspace(m_Previous))
            {
                lexer->result_symbol = m_LastToken = ATTACHED_MODIFIER_END;
                return true;
            }
            break;
        case '[':
            advance();

            if (!iswspace(lexer->lookahead))
            {
                lexer->result_symbol = m_LastToken = LINK_DESCRIPTION_BEGIN;
                return true;
            }
            break;
        case ']':
            advance();

            if (!iswspace(m_Previous))
            {
                lexer->result_symbol = m_LastToken = LINK_DESCRIPTION_END;
                return true;
            }
            break;
        case '{':
            advance();

            if (!iswspace(lexer->lookahead))
            {
                lexer->result_symbol = m_LastToken = LINK_LOCATION_BEGIN;
                m_InLinkLocation = true;
                return true;
            }
            break;
        case '}':
            advance();

            if (is_newline(m_Previous))
            {
                lexer->result_symbol = m_LastToken = NONE;
                return true;
            }

            if (!iswspace(m_Previous))
            {
                lexer->result_symbol = m_LastToken = LINK_LOCATION_END;
                m_InLinkLocation = false;
                return true;
            }
            break;
        }

        if (m_InLinkLocation && check_link_location())
            return true;

        // If we are not in a ranged tag then we should also check for potential
        // attached modifiers, like *this*.
        if (check_attached())
            return true;

        // Match paragraphs
        return parse_text();
    }

    // Skips the next character without including it in the final result.
    void skip()
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        lexer->advance(lexer, true);
    }

    // Advances the lexer forward. The char that was advanced will be returned
    // in the final result.
    void advance()
    {
        m_Previous = m_Current;
        m_Current = lexer->lookahead;
        lexer->advance(lexer, false);
    }

    bool token(const string str)
    {
        for (int32_t c : str)
        {
            if (c == lexer->lookahead)
                advance();
            else
                return false;
        }
        return true;
    }

    /**
     * Checks for the existence of a detached modifier
     * @param results - a list of potential results depending on the amount of consecutive matches
     * found;
     * @param expected - a list of expected modifiers to appear in the sequence.
     */
    [[nodiscard]]
    bool check_detached(const vector<TokenType>& results, const int32_t expected)
    {
        size_t i = m_ParsedChars = 0;
        auto detached_modifier = find(m_DetachedModifiers.begin(), m_DetachedModifiers.end(),
                                      lexer->lookahead);
        do {
            // If the next character is not one we expect then break.
            if (lexer->lookahead != expected)
                break;

            advance();

            // If the next character is whitespace (which is the distinguishing
            // factor between an attached/detached modifier).
            if (is_blank(lexer->lookahead))
            {
                // Retrieve the correct result from the list of provided results
                // depending on how many characters were matched. If we have
                // exceeded the number of results then fall back to the last element.
                size_t max = results.size() - 1;
                TokenType result = results[i <= max ? i : max];

                // Skip all whitespaces.
                while (is_blank(lexer->lookahead))
                    advance();

                lexer->result_symbol = m_LastToken = result;
                reset_active_modifiers();
                return true;
            }

            detached_modifier = find(m_DetachedModifiers.begin(), m_DetachedModifiers.end(),
                                     lexer->lookahead);
            ++i;
            ++m_ParsedChars;
        } while (detached_modifier != m_DetachedModifiers.end());

        // If we've only parsed one character and instantly failed then we might
        // be dealing with an attached modifier!
        if (m_ParsedChars == 1)
        {
            auto found_attached_modifier = m_AttachedModifiers.find(m_Current);
            if (found_attached_modifier != m_AttachedModifiers.end()
                && !m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2])
            {
                m_ActiveModifiers.set((found_attached_modifier->second - BOLD_OPEN) / 2);
                lexer->result_symbol = m_LastToken = found_attached_modifier->second;
                return true;
            }
        }

        return false;
    }

    /**
     * Checks for the existence of an attached modifier
     */
    bool check_attached()
    {
        if (lexer->lookahead == ':')
        {
            bool is_current_char_whitespace = !m_Current || iswspace(m_Current);
            advance();

            if (is_current_char_whitespace || iswspace(lexer->lookahead))
                return false;

            lexer->result_symbol = m_LastToken = LINK_MODIFIER;
            return true;
        }

        auto can_have_modifier = [&]() {
            return (!m_ActiveModifiers[(VERBATIM_OPEN - BOLD_OPEN) / 2]
                    && !m_ActiveModifiers[(INLINE_MATH_OPEN - BOLD_OPEN) / 2]
                    && !m_ActiveModifiers[(INLINE_MACRO_OPEN - BOLD_OPEN) / 2]);
        };

        if (lexer->lookahead == '|')
        {
            advance();

            auto found_attached_modifier = m_AttachedModifiers.find(lexer->lookahead);

            if (m_LastToken >= BOLD_OPEN && m_LastToken <= INLINE_MACRO_CLOSE
                && (m_LastToken % 2) == (BOLD_OPEN % 2))
            {
                if (m_LastToken != VERBATIM_OPEN && m_LastToken != INLINE_MACRO_OPEN
                    && m_LastToken != INLINE_MATH_OPEN && !can_have_modifier())
                    return false;

                lexer->result_symbol = m_LastToken = FREE_FORM_MODIFIER_OPEN;
                return true;
            }
            else if (found_attached_modifier != m_AttachedModifiers.end())
            {
                if (!can_have_modifier()
                    && !(found_attached_modifier->second == VERBATIM_OPEN
                         && m_ActiveModifiers[(VERBATIM_OPEN - BOLD_OPEN) / 2])
                    && !(found_attached_modifier->second == INLINE_MATH_OPEN
                         && m_ActiveModifiers[(INLINE_MATH_OPEN - BOLD_OPEN) / 2])
                    && !(found_attached_modifier->second == INLINE_MACRO_OPEN
                         && m_ActiveModifiers[(INLINE_MACRO_OPEN - BOLD_OPEN) / 2]))
                    return false;
                lexer->result_symbol = m_LastToken = FREE_FORM_MODIFIER_CLOSE;
                return true;
            }
            else
            {
                lexer->result_symbol = m_LastToken = WORD;
                return true;
            }
        }

        auto found_attached_modifier = m_AttachedModifiers.find(lexer->lookahead);

        if (found_attached_modifier == m_AttachedModifiers.end())
            return false;

        // First check for the existence of an opening attached modifier
        if (iswspace(m_Current)
            || (iswpunct(m_Current) && m_LastToken != FREE_FORM_MODIFIER_CLOSE)
            || !m_Current)
        {
            advance();

            // empty attached modifier
            if (lexer->lookahead == found_attached_modifier->first)
            {
                while (lexer->lookahead == found_attached_modifier->first)
                    advance();
                return false;
            }

            auto found_previous_attached_modifier = m_AttachedModifiers.find(m_Previous);

            if (!iswspace(lexer->lookahead)
                && !m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2]
                && can_have_modifier())
            {
                m_ActiveModifiers.set((found_attached_modifier->second - BOLD_OPEN) / 2);
                lexer->result_symbol = m_LastToken = found_attached_modifier->second;
                return true;
            }
        }
        else
            advance();

        if (lexer->lookahead == found_attached_modifier->first)
        {
            while (lexer->lookahead == found_attached_modifier->first)
                advance();
            return false;
        }

        auto found_next_attached_modifier = m_AttachedModifiers.find(lexer->lookahead);

        if (found_next_attached_modifier != m_AttachedModifiers.end())
        {
            m_ActiveModifiers.reset((found_attached_modifier->second - BOLD_OPEN) / 2);
            lexer->result_symbol = m_LastToken =
                static_cast<TokenType>(found_attached_modifier->second + 1);
            return true;
        }

        if ((!iswspace(m_Previous) || !m_Previous)
            && (iswspace(lexer->lookahead) || iswpunct(lexer->lookahead) || !lexer->lookahead))
        {
            m_ActiveModifiers.reset((found_attached_modifier->second - BOLD_OPEN) / 2);
            lexer->result_symbol = m_LastToken =
                static_cast<TokenType>(found_attached_modifier->second + 1);
            return true;
        }

        return false;
    }

    /// Attempts to parse a link location.
    bool check_link_location()
    {
        size_t count = 0;

        switch (m_LastToken) {
        case LINK_LOCATION_BEGIN:
            if (lexer->lookahead == ':')
            {
                lexer->result_symbol = m_LastToken = LINK_FILE_BEGIN;
                advance();
                return !iswspace(lexer->lookahead);
            }
            // since we have no break here, if we do not detect a beginning of a
            // file segment we fall through into this next case statement
        case INTERSECTING_MODIFIER:
        case LINK_FILE_END:
            switch (lexer->lookahead) {
            case '?':
                lexer->result_symbol = m_LastToken = LINK_TARGET_WIKI;
                break;
            case '#':
                lexer->result_symbol = m_LastToken = LINK_TARGET_GENERIC;
                break;
            case '/':
                if (m_LastToken == LINK_FILE_END)
                    return false;
                lexer->result_symbol = m_LastToken = LINK_TARGET_EXTERNAL_FILE;
                break;
            case '@':
                if (m_LastToken == LINK_FILE_END)
                    return false;
                lexer->result_symbol = m_LastToken = LINK_TARGET_TIMESTAMP;
                break;
            case '$':
                lexer->result_symbol = m_LastToken = LINK_TARGET_DEFINITION;
                break;
            case '^':
                lexer->result_symbol = m_LastToken = LINK_TARGET_FOOTNOTE;
                break;
            case '*':
                advance();

                while (lexer->lookahead == '*')
                {
                    ++count;
                    advance();
                }

                lexer->result_symbol = m_LastToken =
                    static_cast<TokenType>(LINK_TARGET_HEADING1 + (count <= 5 ? count : 5));

                if (!iswspace(lexer->lookahead))
                    return false;

                while (iswspace(lexer->lookahead))
                    advance();

                return true;
            default:
                lexer->result_symbol = m_LastToken = iswdigit(lexer->lookahead) ?
                                                     LINK_TARGET_LINE_NUMBER : LINK_TARGET_URL;
                return true;
            }

            advance();

            if (!iswspace(lexer->lookahead))
                return false;

            while (iswspace(lexer->lookahead))
                advance();

            return true;
        case LINK_FILE_BEGIN:
            while (lexer->lookahead)
            {
                if (lexer->lookahead == ':' && m_Current != '\\')
                    break;

                // bail when potentially dealing with verbatim
                if (lexer->lookahead == '`')
                    return false;

                // bail when potentially dealing with inline comments
                if (lexer->lookahead == '%')
                    return false;

                // bail when potentially dealing with inline macro invocations
                if (lexer->lookahead == '&')
                    return false;

                // bail when potentially dealing with inline math
                // Here we exclude the potential beginning of the file beginning
                // because this is how we denote links to other workspaces.
                if (lexer->lookahead == '$' && m_Current != ':')
                    return false;

                advance();
            }

            lexer->result_symbol = m_LastToken = LINK_FILE_TEXT;
            return true;
        case LINK_FILE_TEXT:
            if (lexer->lookahead == ':')
            {
                lexer->result_symbol = m_LastToken = LINK_FILE_END;
                advance();
                switch (lexer->lookahead) {
                case '}':
                case '#':
                case '%':
                case '$':
                case '^':
                case '*':
                    return true;
                default:
                    return iswdigit(lexer->lookahead);
                }
            }
        default:
            return false;
        }
    }

    /// Attempts to parse a detached modifier extension.
    bool check_detached_mod_extension()
    {
        switch (m_LastToken) {
        case DETACHED_MODIFIER_EXTENSION_BEGIN:
        case MODIFIER_EXTENSION_DELIMITER:
            switch (lexer->lookahead) {
            case '#':
                lexer->result_symbol = m_LastToken = PRIORITY;
                break;
            case '@':
                lexer->result_symbol = m_LastToken = TIMESTAMP;
                break;
            case ' ':
            case '\t':
            case '\v':
                lexer->result_symbol = m_LastToken = TODO_ITEM_UNDONE;
                break;
            case '-':
                lexer->result_symbol = m_LastToken = TODO_ITEM_PENDING;
                break;
            case 'x':
                lexer->result_symbol = m_LastToken = TODO_ITEM_DONE;
                break;
            case '=':
                lexer->result_symbol = m_LastToken = TODO_ITEM_ON_HOLD;
                break;
            case '_':
                lexer->result_symbol = m_LastToken = TODO_ITEM_CANCELLED;
                break;
            case '!':
                lexer->result_symbol = m_LastToken = TODO_ITEM_URGENT;
                break;
            case '?':
                lexer->result_symbol = m_LastToken = TODO_ITEM_UNCERTAIN;
                break;
            case '+':
                lexer->result_symbol = m_LastToken = TODO_ITEM_RECURRING;
                break;
            default:
                advance();
                return false;
            }

            advance();

            while (iswspace(lexer->lookahead))
                advance();

            return true;
        case TIMESTAMP:
        case PRIORITY:
        case TODO_ITEM_RECURRING:
            switch (lexer->lookahead) {
            case ')':
                advance();
                lexer->result_symbol = m_LastToken = DETACHED_MODIFIER_EXTENSION_END;
                return true;
            case '|':
                advance();
                lexer->result_symbol = m_LastToken = MODIFIER_EXTENSION_DELIMITER;
                return true;
            }

            while (lexer->lookahead && lexer->lookahead != '|' && lexer->lookahead != ')')
                advance();

            lexer->result_symbol = m_LastToken =
                (m_LastToken == TIMESTAMP || m_LastToken == TODO_ITEM_RECURRING) ?
                    TIMESTAMP_DATA : PRIORITY_DATA;

            return true;
        case TODO_ITEM_UNDONE:
        case TODO_ITEM_PENDING:
        case TODO_ITEM_DONE:
        case TODO_ITEM_ON_HOLD:
        case TODO_ITEM_CANCELLED:
        case TODO_ITEM_URGENT:
        case TODO_ITEM_UNCERTAIN:
        case TIMESTAMP_DATA:
        case PRIORITY_DATA:
            switch (lexer->lookahead) {
            case ')':
                advance();
                lexer->result_symbol = m_LastToken = DETACHED_MODIFIER_EXTENSION_END;
                return true;
            case '|':
                if (m_AttachedModifiers.find(m_Current) == m_AttachedModifiers.end())
                {
                    advance();
                    lexer->result_symbol = m_LastToken = MODIFIER_EXTENSION_DELIMITER;
                    return true;
                }
            }
            return false;
        default:
            if (m_LastToken < HEADING1 || m_LastToken > MULTI_TABLE_CELL_SUFFIX)
                return false;

            switch (lexer->lookahead) {
            case '(':
                advance();
                lexer->result_symbol = m_LastToken = DETACHED_MODIFIER_EXTENSION_BEGIN;
                return true;
            case ')':
                advance();
                lexer->result_symbol = m_LastToken = DETACHED_MODIFIER_EXTENSION_END;
                return true;
            }
        }

        return false;
    }

    /**
     * Simply parses any word (segment containing consecutive non-whitespace
     * characters). If in a tag parse_text parses till a newline is encountered.
     */
    bool parse_text()
    {
        if (m_TagContext == TagType::IN_VERBATIM_TAG)
        {
            while (!is_newline(lexer->lookahead))
                advance();
            lexer->result_symbol = m_LastToken = WORD;
            return true;
        }

        if (((char)m_TagContext % 2) == 0 && lexer->lookahead == '.')
        {
            advance();
            lexer->result_symbol = m_LastToken = TAG_DELIMITER;
            return true;
        }

        if (is_newline(lexer->lookahead))
        {
            lexer->result_symbol = m_LastToken = WORD;
            return true;
        }

        if (is_blank(lexer->lookahead))
        {
            do
                advance();
            while (is_blank(lexer->lookahead));

            if (lexer->lookahead == ':')
            {
                advance();
                if (is_blank(lexer->lookahead))
                {
                    advance();
                    lexer->result_symbol = m_LastToken = INTERSECTING_MODIFIER;
                    return true;
                }
                else
                {
                    lexer->result_symbol = m_LastToken = WORD;
                    return true;
                }
            }

            lexer->result_symbol = m_LastToken = SPACE;
            return true;
        }

        const TokenType resulting_symbol = iswupper(lexer->lookahead) ?
                                           CAPITALIZED_WORD : WORD;

        do
        {
            bool _break = false;
            switch (lexer->lookahead)
            case ':': case '|': case '~': case '\\':
            case '<': case '>':
            case '[': case ']':
            case '{': case '}':
            case '(': case ')':
                _break = true;

            if (_break
                || m_AttachedModifiers.find(lexer->lookahead) != m_AttachedModifiers.end()
                || ((char)m_TagContext % 2 == 0 && lexer->lookahead == '.'))
                break;
            else
                advance();
        } while (lexer->lookahead && !iswspace(lexer->lookahead) && lexer->lookahead != '\\');

        lexer->result_symbol = m_LastToken = resulting_symbol;
        return true;
    }

    inline void reset_active_modifiers() { m_ActiveModifiers.reset(); }

    inline bool is_newline(int32_t c) { return !c || c == '\n' || c == '\r'; }

    inline bool is_blank(int32_t c) { return c && std::iswblank(c); }
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

    bool tree_sitter_norg_external_scanner_scan(void* payload,
                                                TSLexer* lexer,
                                                const bool* valid_symbols)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);
        scanner->lexer = lexer;
        return scanner->scan(valid_symbols);
    }

    unsigned tree_sitter_norg_external_scanner_serialize(void* payload, char* buffer)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);

        if (8 + scanner->m_ActiveModifiers.size() >= TREE_SITTER_SERIALIZATION_BUFFER_SIZE)
            return 0;

        buffer[0] = scanner->m_LastToken;
        buffer[1] = scanner->m_TagLevel;
        buffer[2] = (char)scanner->m_TagContext;
        buffer[3] = scanner->m_InLinkLocation;

        // Copy 4 bytes from int32_t to buffer 2 to 6 positions.
        memcpy(buffer + 4, &scanner->m_Current, 4);

        // Serialize the attached modifier bitset into the char array
        // We cast it down to a uint32_t because we genuinely won't be using any
        // more than that.
        for (int i = 0; i < scanner->m_ActiveModifiers.size(); ++i)
            buffer[8 + i] = scanner->m_ActiveModifiers[i];

        return 8 + scanner->m_ActiveModifiers.size();
    }

    void tree_sitter_norg_external_scanner_deserialize(void* payload,
                                                       const char* buffer,
                                                       unsigned length)
    {
        Scanner* scanner = static_cast<Scanner*>(payload);

        if (length == 0)
        {
            scanner->m_TagLevel = 0;
            scanner->m_TagContext = TagType::NONE;
            scanner->m_InLinkLocation = false;
            scanner->m_LastToken = NONE;
            scanner->m_Current = 0;
            scanner->m_ActiveModifiers = 0;
            return;
        }

        scanner->m_LastToken = (TokenType)buffer[0];
        scanner->m_TagLevel = (size_t)buffer[1];
        scanner->m_TagContext = (TagType)buffer[2];
        scanner->m_InLinkLocation = (bool)buffer[3];

        memcpy(&scanner->m_Current, buffer + 4, 4);

        for (int i = 0; i < scanner->m_ActiveModifiers.size(); ++i)
            scanner->m_ActiveModifiers[i] = buffer[8 + i];
    }
}
