#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>
#include "tree_sitter/parser.h"

typedef enum
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
} TokenType;

/* This was `enum class TagType : char` in the C++ original. C has no scoped
 * enums, so the members are given a `TAG_` prefix to avoid clashing with
 * anything else in this translation unit. */
typedef enum
{
    TAG_NONE = 1,

    TAG_ON_TAG,
    TAG_IN_TAG,

    TAG_ON_VERBATIM_TAG,
    TAG_IN_VERBATIM_TAG,
} TagType;

/* Stand-in for `std::unordered_map<int32_t, TokenType>::value_type` /
 * iterator pair (`->first`, `->second`) used throughout the original file. */
typedef struct
{
    int32_t first;
    TokenType second;
} ModifierEntry;

/* Number of bits tracked by the original `std::bitset<((INLINE_MACRO_OPEN -
 * BOLD_OPEN) / 2) + 1>`. BOLD_OPEN .. INLINE_MACRO_OPEN spans 11
 * open/close modifier pairs, so this bitset needs 11 slots. */
#define ACTIVE_MODIFIERS_SIZE (((INLINE_MACRO_OPEN - BOLD_OPEN) / 2) + 1)

typedef struct
{
    TSLexer* lexer;

    int32_t
        m_Previous, /* previous char */
        m_Current;  /* current char */

    TagType m_TagContext;
    size_t m_TagLevel;

    bool m_InLinkLocation;

    /* The last matched token type (used to detect things like todo items
     * which require an unordered list prefix beforehand) */
    TokenType m_LastToken;

    /* Used for lookback */
    size_t m_ParsedChars;

    /* Was a `std::bitset<...>` in the original; a plain bool array behaves
     * identically for the indexed get/set/reset usage in this file. */
    bool m_ActiveModifiers[ACTIVE_MODIFIERS_SIZE];
} Scanner;

/* Equivalent of the original `const array<int32_t, 12> m_DetachedModifiers`.
 * This never changes between scanner instances, so it is a plain file-scope
 * constant rather than a per-instance field. */
static const int32_t DETACHED_MODIFIERS[12] = {
    '*',  /* Headings */
    '-',  /* Unordered Lists */
    '>',  /* Quotes */
    '%',  /* Attributes */
    '=',
    '~',  /* Ordered Lists */
    '$',  /* Definitions */
    '_',
    '^',  /* Footnotes */
    '&',
    '<',
    ':',  /* Table cells */
};
#define DETACHED_MODIFIERS_COUNT \
    (sizeof(DETACHED_MODIFIERS) / sizeof(DETACHED_MODIFIERS[0]))

/* Equivalent of the original `const unordered_map<int32_t, TokenType>
 * m_AttachedModifiers`. */
static const ModifierEntry ATTACHED_MODIFIERS[] = {
    {'*', BOLD_OPEN},        {'/', ITALIC_OPEN},       {'-', STRIKETHROUGH_OPEN},
    {'_', UNDERLINE_OPEN},   {'!', SPOILER_OPEN},      {'`', VERBATIM_OPEN},
    {'^', SUPERSCRIPT_OPEN}, {',', SUBSCRIPT_OPEN},    {'%', INLINE_COMMENT_OPEN},
    {'$', INLINE_MATH_OPEN}, {'&', INLINE_MACRO_OPEN},
};
#define ATTACHED_MODIFIERS_COUNT \
    (sizeof(ATTACHED_MODIFIERS) / sizeof(ATTACHED_MODIFIERS[0]))

/* Result tables used with scanner_check_detached(), equivalent to the
 * `vector<TokenType>` initializer-list literals passed at each call site in
 * the original C++. */
static const TokenType HEADING_RESULTS[] = {
    HEADING1, HEADING2, HEADING3, HEADING4, HEADING5, HEADING6};
static const TokenType QUOTE_RESULTS[] = {
    QUOTE1, QUOTE2, QUOTE3, QUOTE4, QUOTE5, QUOTE6};
static const TokenType UNORDERED_LIST_RESULTS[] = {
    UNORDERED_LIST1, UNORDERED_LIST2, UNORDERED_LIST3,
    UNORDERED_LIST4, UNORDERED_LIST5, UNORDERED_LIST6};
static const TokenType ORDERED_LIST_RESULTS[] = {
    ORDERED_LIST1, ORDERED_LIST2, ORDERED_LIST3,
    ORDERED_LIST4, ORDERED_LIST5, ORDERED_LIST6};
static const TokenType DEFINITION_RESULTS[] = {SINGLE_DEFINITION, MULTI_DEFINITION, NONE};
static const TokenType FOOTNOTE_RESULTS[] = {SINGLE_FOOTNOTE, MULTI_FOOTNOTE, NONE};
static const TokenType TABLE_CELL_RESULTS[] = {SINGLE_TABLE_CELL, MULTI_TABLE_CELL, NONE};
static const TokenType UNDERSCORE_RESULTS[] = {NONE, NONE};

#define RESULTS_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

/* ---- forward declarations of "methods" (all take Scanner* as `self`) ---- */

static bool scan(Scanner* s, const bool* valid_symbols);
static bool parse_text(Scanner* s);
static void skip(Scanner* s);
static void advance(Scanner* s);
static bool scanner_token(Scanner* s, const char* str);
static bool scanner_check_detached(Scanner* s, const TokenType* results,
                                   size_t results_count, int32_t expected);
static bool check_attached(Scanner* s);
static bool check_link_location(Scanner* s);
static bool check_detached_mod_extension(Scanner* s);
static void reset_active_modifiers(Scanner* s);
static inline bool is_newline(int32_t c);
static inline bool is_blank(int32_t c);
static const ModifierEntry* find_attached_modifier(int32_t c);
static bool is_detached_modifier(int32_t c);
static bool can_have_modifier(const Scanner* s);

/* ------------------------------------------------------------------------ */

static const ModifierEntry* find_attached_modifier(int32_t c)
{
    for (size_t i = 0; i < ATTACHED_MODIFIERS_COUNT; ++i)
        if (ATTACHED_MODIFIERS[i].first == c)
            return &ATTACHED_MODIFIERS[i];
    return NULL;
}

static bool is_detached_modifier(int32_t c)
{
    for (size_t i = 0; i < DETACHED_MODIFIERS_COUNT; ++i)
        if (DETACHED_MODIFIERS[i] == c)
            return true;
    return false;
}

/* Was a capturing lambda (`can_have_modifier`) declared inside check_attached()
 * in the original; C has no closures, so it is a free function that takes
 * the scanner explicitly. */
static bool can_have_modifier(const Scanner* s)
{
    return !s->m_ActiveModifiers[(VERBATIM_OPEN - BOLD_OPEN) / 2]
        && !s->m_ActiveModifiers[(INLINE_MATH_OPEN - BOLD_OPEN) / 2]
        && !s->m_ActiveModifiers[(INLINE_MACRO_OPEN - BOLD_OPEN) / 2];
}

static bool scan(Scanner* s, const bool* valid_symbols)
{
    (void)valid_symbols;

    s->lexer->result_symbol = NONE;

    /* Are we at the end of file? If so, bail. */
    if (s->lexer->eof(s->lexer) || !s->lexer->lookahead)
    {
        reset_active_modifiers(s);
        return false;
    }

    if (s->m_LastToken == TRAILING_MODIFIER)
    {
        advance(s);
        return parse_text(s);
    }
    else if (is_newline(s->lexer->lookahead))
    {
        advance(s);

        s->lexer->result_symbol = s->m_LastToken = LINE_BREAK;

        if (s->lexer->eof(s->lexer) || !s->lexer->lookahead)
        {
            reset_active_modifiers(s);
            return true;
        }

        if (s->m_TagContext != TAG_NONE && ((char)s->m_TagContext % 2) == 0)
        {
            s->m_TagContext = (TagType)((char)s->m_TagContext + 1);
            return true;
        }

        if (is_newline(s->lexer->lookahead))
        {
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = PARAGRAPH_BREAK;
            reset_active_modifiers(s);
        }

        return true;
    }

    /* If we're at the beginning of a line check for all detached modifiers */
    if (s->lexer->get_column(s->lexer) == 0)
    {
        /* Skip all leading whitespace */
        while (is_blank(s->lexer->lookahead))
            skip(s);

        /* We are dealing with a ranged verbatim tag: @something */
        if (s->lexer->lookahead == '@')
        {
            advance(s);

            /* Mark the end of the token here
             * We do this because we only want the returned token to be part
             * of the `@` symbol, not the symbol + the name */
            s->lexer->mark_end(s->lexer);

            /* Check whether the tag is `@end`. */
            if (scanner_token(s, "end") && (iswspace(s->lexer->lookahead) || !s->lexer->lookahead))
            {
                while (is_blank(s->lexer->lookahead))
                    advance(s);

                if ((iswspace(s->lexer->lookahead) || !s->lexer->lookahead)
                    && s->m_TagContext == TAG_IN_VERBATIM_TAG)
                {
                    s->lexer->result_symbol = s->m_LastToken = RANGED_VERBATIM_TAG_END;
                    s->m_TagContext = TAG_NONE;
                    return true;
                }

                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }

            /* This is a fallback. If the tag ends up not being `@end` then... */
            if (s->m_LastToken == RANGED_VERBATIM_TAG || s->m_TagContext == TAG_IN_VERBATIM_TAG)
            {
                /* Ignore the char if we are already inside of a ranged tag. */
                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }

            s->lexer->result_symbol = s->m_LastToken = RANGED_VERBATIM_TAG;
            s->m_TagContext = TAG_ON_VERBATIM_TAG;
            return true;
        }

        if (s->m_TagContext == TAG_IN_VERBATIM_TAG)
            return parse_text(s);

        /* We are dealing with a macro tag (=something) */
        if (s->lexer->lookahead == '=' && s->m_TagContext != TAG_IN_VERBATIM_TAG)
        {
            advance(s);

            /* Mark the end of the token here
             * We do this because we only want the returned token to be part
             * of the `=` symbol, not the symbol + the name */
            s->lexer->mark_end(s->lexer);

            /* These sets of checks check whether the tag is `=end` */
            if (scanner_token(s, "end") && (iswspace(s->lexer->lookahead) || !s->lexer->lookahead))
            {
                while (s->lexer->lookahead && iswspace(s->lexer->lookahead)
                       && !is_newline(s->lexer->lookahead))
                    advance(s);

                if ((iswspace(s->lexer->lookahead) || !s->lexer->lookahead) && s->m_TagLevel)
                {
                    s->lexer->result_symbol = s->m_LastToken = MACRO_TAG_END;
                    --s->m_TagLevel;
                    return true;
                }

                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }
            else if (s->lexer->lookahead == '=')
            {
                advance(s);
                if (s->lexer->lookahead == '=')
                {
                    /* we are now three-characters in */
                    do
                        advance(s);
                    while (s->lexer->lookahead == '=');

                    if (is_newline(s->lexer->lookahead))
                    {
                        /* reset the marked end */
                        s->lexer->mark_end(s->lexer);
                        advance(s);
                        s->lexer->result_symbol = s->m_LastToken = STRONG_PARAGRAPH_DELIMITER;
                        return true;
                    }
                    else
                    {
                        /* reset the marked end */
                        s->lexer->mark_end(s->lexer);
                        advance(s);
                        s->lexer->result_symbol = s->m_LastToken = WORD;
                        return true;
                    }
                }
                else
                {
                    s->lexer->mark_end(s->lexer);
                    s->lexer->result_symbol = s->m_LastToken = WORD;
                    return true;
                }
            }

            /* This is a fallback. If the tag ends up not being `=end` then... */
            if (s->m_LastToken == MACRO_TAG)
            {
                /* Ignore the char if we are already inside of a ranged tag. */
                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }

            /* or push back the indentation level and return. */
            s->lexer->result_symbol = s->m_LastToken = MACRO_TAG;
            s->m_TagContext = TAG_ON_TAG;
            ++s->m_TagLevel;
            return true;
        }
        /* We are dealing with a ranged tag (|something) */
        else if (s->lexer->lookahead == '|' && s->m_TagContext != TAG_IN_VERBATIM_TAG)
        {
            advance(s);

            /* Mark the end of the token here.
             * We do this because we only want the returned token to be part
             * of the `|` symbol, not the symbol + the name. */
            s->lexer->mark_end(s->lexer);

            /* These sets of checks check whether the tag is `|end`. */
            if (scanner_token(s, "end") && (iswspace(s->lexer->lookahead) || !s->lexer->lookahead))
            {
                while (is_blank(s->lexer->lookahead))
                    advance(s);

                if ((iswspace(s->lexer->lookahead) || !s->lexer->lookahead) && s->m_TagLevel)
                {
                    s->lexer->result_symbol = s->m_LastToken = RANGED_TAG_END;
                    --s->m_TagLevel;
                    return true;
                }

                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }

            /* This is a fallback. If the tag ends up not being `|end` then... */
            if (s->m_LastToken == RANGED_TAG)
            {
                /* Ignore the char if we are already inside of a ranged tag. */
                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }

            /* Or push back the indentation level and return. */
            s->lexer->result_symbol = s->m_LastToken = RANGED_TAG;
            s->m_TagContext = TAG_ON_TAG;
            ++s->m_TagLevel;
            return true;
        }
        /* we are dealing with a strong carryover (#something) */
        else if (s->lexer->lookahead == '#' && s->m_TagContext != TAG_IN_VERBATIM_TAG)
        {
            advance(s);

            if (!s->lexer->lookahead || iswspace(s->lexer->lookahead))
            {
                if (is_newline(s->lexer->lookahead))
                    s->lexer->result_symbol = s->m_LastToken = INDENT_SEGMENT;
                else
                    s->lexer->result_symbol = s->m_LastToken = WORD;

                return true;
            }

            s->lexer->result_symbol = s->m_LastToken = STRONG_CARRYOVER;
            return true;
        }
        /* we are dealing with a weak carryover (+something) */
        else if (s->lexer->lookahead == '+' && s->m_TagContext != TAG_IN_VERBATIM_TAG)
        {
            advance(s);
            if (s->lexer->lookahead != '+')
            {
                s->lexer->result_symbol = s->m_LastToken = WEAK_CARRYOVER;
                return true;
            }
        }
        /* we are dealing with a infirm tag (.something) */
        else if (s->lexer->lookahead == '.' && s->m_TagContext != TAG_IN_VERBATIM_TAG)
        {
            advance(s);
            if (s->lexer->lookahead != '.')
            {
                s->lexer->result_symbol = s->m_LastToken = INFIRM_TAG;
                return true;
            }
        }

        /* The idea of the check_detached function is as follows:
         * We check for the '*' character and depending on how many we
         * encounter we return a different token If we encounter 1 '*'
         * char then return heading1, if we encounter two in a row then
         * return heading2, you get the idea. If we encounter more than
         * the given amount of fallbacks then the last fallback will
         * always be chosen. This means that if we have 7 consecutive
         * '*' chars then we will still fall back to the HEADING6 token
         * instead. */
        if (scanner_check_detached(s, HEADING_RESULTS, RESULTS_COUNT(HEADING_RESULTS), '*'))
            return true;

        /* Check for the existence of quotes */
        if (scanner_check_detached(s, QUOTE_RESULTS, RESULTS_COUNT(QUOTE_RESULTS), '>'))
            return true;

        /* Check for the existence of an unordered list element. */
        if (scanner_check_detached(s, UNORDERED_LIST_RESULTS, RESULTS_COUNT(UNORDERED_LIST_RESULTS), '-'))
            return true;
        /* If we end up failing to parse an unordered modifier not all
         * hope is lost. We also have a weak paragraph delimiter which
         * looks like this:
         * ---
         * Detached modifiers are valid only if there is at least one
         * bit of whitespace after themselves. A delimiting modifier
         * strictly does not allow whitespace after itself. That's how
         * we differentiate between the two. m_ParsedChars is
         * incremented every time `check_detached` successfully parses a
         * character. We can use this to our advantage! The parser will
         * encounter 3 consecutive '-' chars and will parse all the way
         * up until the end. It will then try to return UNORDERED_LIST3
         * but will fail because there won't be any whitespace after the
         * 3 chars. It will then return NONE. Even though it may have
         * failed the m_ParsedChars value has still been modified! If
         * m_ParsedChars is 3 then that means we have parsed '---' and
         * hence we return a WEAK_PARAGRAPH_DELIMITER. This check is
         * even further enforced by checking if the next char is a
         * newline, which makes sense considering the parser head:
         * ---
         *   ^ will be here, and lexer->lookahead will return '\n' */
        else if (is_newline(s->lexer->lookahead) && s->m_ParsedChars >= 3)
        {
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = WEAK_PARAGRAPH_DELIMITER;
            return true;
        }

        if (scanner_check_detached(s, ORDERED_LIST_RESULTS, RESULTS_COUNT(ORDERED_LIST_RESULTS), '~'))
            return true;
        else if (is_newline(s->lexer->lookahead) && s->m_ParsedChars == 1)
        {
            if (s->lexer->eof(s->lexer))
            {
                reset_active_modifiers(s);
                return false;
            }

            s->lexer->result_symbol = s->m_LastToken = TRAILING_MODIFIER;
            return true;
        }

        if (scanner_check_detached(s, DEFINITION_RESULTS, RESULTS_COUNT(DEFINITION_RESULTS), '$'))
            return true;
        else if (is_newline(s->lexer->lookahead) && s->m_ParsedChars == 2)
        {
            advance(s);
            s->lexer->result_symbol = MULTI_DEFINITION_SUFFIX;
            return true;
        }

        if (scanner_check_detached(s, FOOTNOTE_RESULTS, RESULTS_COUNT(FOOTNOTE_RESULTS), '^'))
            return true;
        else if (is_newline(s->lexer->lookahead) && s->m_ParsedChars == 2)
        {
            advance(s);
            s->lexer->result_symbol = MULTI_FOOTNOTE_SUFFIX;
            return true;
        }

        if (scanner_check_detached(s, TABLE_CELL_RESULTS, RESULTS_COUNT(TABLE_CELL_RESULTS), ':'))
            return true;
        else if (is_newline(s->lexer->lookahead) && s->m_ParsedChars == 2)
        {
            advance(s);
            s->lexer->result_symbol = MULTI_TABLE_CELL_SUFFIX;
            return true;
        }

        if (scanner_check_detached(s, UNDERSCORE_RESULTS, RESULTS_COUNT(UNDERSCORE_RESULTS), '_'))
            return true;
        else if (is_newline(s->lexer->lookahead) && s->m_ParsedChars >= 3)
        {
            s->lexer->result_symbol = s->m_LastToken = HORIZONTAL_LINE;
            return true;
        }
    }

    switch (s->lexer->lookahead) {
    case '~':
        advance(s);
        s->lexer->mark_end(s->lexer);

        if (is_newline(s->lexer->lookahead))
        {
            advance(s);
            if (s->lexer->eof(s->lexer))
            {
                reset_active_modifiers(s);
                return false;
            }
            s->lexer->result_symbol = s->m_LastToken = TRAILING_MODIFIER;
            return true;
        }

        return parse_text(s);
    case '\\':  /* Check for an escape seqence (e.g. "\*") */
        advance(s);
        s->lexer->result_symbol = s->m_LastToken = ESCAPE_SEQUENCE;
        return true;
    }

    if (check_detached_mod_extension(s))
        return true;
    else if (((s->m_LastToken >= HEADING1 && s->m_LastToken <= MULTI_TABLE_CELL_SUFFIX)
              || s->m_LastToken == DETACHED_MODIFIER_EXTENSION_END)
             && s->lexer->lookahead == ':')
    {
        advance(s);
        bool is_indent_segment = false;

        if (s->lexer->lookahead == ':')
        {
            advance(s);
            is_indent_segment = true;
        }

        if (!is_newline(s->lexer->lookahead))
        {
            s->lexer->result_symbol = s->m_LastToken = WORD;
            return true;
        }

        /* Move past the newline character as well */
        advance(s);

        s->lexer->result_symbol = s->m_LastToken = (TokenType)(SLIDE + is_indent_segment);
        return true;
    }

    switch (s->lexer->lookahead) {
    case '<':
        advance(s);

        if (!iswspace(s->lexer->lookahead))
        {
            s->lexer->result_symbol = s->m_LastToken = INLINE_LINK_TARGET_OPEN;
            s->m_InLinkLocation = true;
            return true;
        }
        break;
    case '>':
        advance(s);

        if (!iswspace(s->m_Previous) && s->m_LastToken != LINK_LOCATION_BEGIN
            && s->m_LastToken != LINK_FILE_END)
        {
            s->lexer->result_symbol = s->m_LastToken = INLINE_LINK_TARGET_CLOSE;
            s->m_InLinkLocation = false;
            return true;
        }
        break;
    case '(':
        advance(s);

        if (!iswspace(s->lexer->lookahead) && s->m_LastToken != NONE
            && ((s->m_LastToken >= BOLD_OPEN && s->m_LastToken <= INLINE_MACRO_CLOSE
                 && (s->m_LastToken % 2) == (BOLD_CLOSE % 2))
                || s->m_LastToken == LINK_DESCRIPTION_END
                || s->m_LastToken == LINK_LOCATION_END
                || s->m_LastToken == INLINE_LINK_TARGET_CLOSE))
        {
            s->lexer->result_symbol = s->m_LastToken = ATTACHED_MODIFIER_BEGIN;
            return true;
        }
        else
        {
            s->lexer->result_symbol = s->m_LastToken = WORD;
            return true;
        }
        break;
    case ')':
        advance(s);

        if (!iswspace(s->m_Previous))
        {
            s->lexer->result_symbol = s->m_LastToken = ATTACHED_MODIFIER_END;
            return true;
        }
        break;
    case '[':
        advance(s);

        if (!iswspace(s->lexer->lookahead))
        {
            s->lexer->result_symbol = s->m_LastToken = LINK_DESCRIPTION_BEGIN;
            return true;
        }
        break;
    case ']':
        advance(s);

        if (!iswspace(s->m_Previous))
        {
            s->lexer->result_symbol = s->m_LastToken = LINK_DESCRIPTION_END;
            return true;
        }
        break;
    case '{':
        advance(s);

        if (!iswspace(s->lexer->lookahead))
        {
            s->lexer->result_symbol = s->m_LastToken = LINK_LOCATION_BEGIN;
            s->m_InLinkLocation = true;
            return true;
        }
        break;
    case '}':
        advance(s);

        if (is_newline(s->m_Previous))
        {
            s->lexer->result_symbol = s->m_LastToken = NONE;
            return true;
        }

        if (!iswspace(s->m_Previous))
        {
            s->lexer->result_symbol = s->m_LastToken = LINK_LOCATION_END;
            s->m_InLinkLocation = false;
            return true;
        }
        break;
    }

    if (s->m_InLinkLocation && check_link_location(s))
        return true;

    /* If we are not in a ranged tag then we should also check for potential
     * attached modifiers, like *this*. */
    if (check_attached(s))
        return true;

    /* Match paragraphs */
    return parse_text(s);
}

/* Skips the next character without including it in the final result. */
static void skip(Scanner* s)
{
    s->m_Previous = s->m_Current;
    s->m_Current = s->lexer->lookahead;
    s->lexer->advance(s->lexer, true);
}

/* Advances the lexer forward. The char that was advanced will be returned
 * in the final result. */
static void advance(Scanner* s)
{
    s->m_Previous = s->m_Current;
    s->m_Current = s->lexer->lookahead;
    s->lexer->advance(s->lexer, false);
}

static bool scanner_token(Scanner* s, const char* str)
{
    for (const char* p = str; *p; ++p)
    {
        if ((int32_t)(unsigned char)*p == s->lexer->lookahead)
            advance(s);
        else
            return false;
    }
    return true;
}

/**
 * Checks for the existence of a detached modifier
 * @param results - a list of potential results depending on the amount of consecutive matches
 * found;
 * @param results_count - the number of entries in `results`.
 * @param expected - the expected modifier char to appear in the sequence.
 */
static bool scanner_check_detached(Scanner* s, const TokenType* results,
                                   size_t results_count, int32_t expected)
{
    size_t i = s->m_ParsedChars = 0;
    bool detached_modifier = is_detached_modifier(s->lexer->lookahead);
    do {
        /* If the next character is not one we expect then break. */
        if (s->lexer->lookahead != expected)
            break;

        advance(s);

        /* If the next character is whitespace (which is the distinguishing
         * factor between an attached/detached modifier). */
        if (is_blank(s->lexer->lookahead))
        {
            /* Retrieve the correct result from the list of provided results
             * depending on how many characters were matched. If we have
             * exceeded the number of results then fall back to the last element. */
            size_t max = results_count - 1;
            TokenType result = results[i <= max ? i : max];

            /* Skip all whitespaces. */
            while (is_blank(s->lexer->lookahead))
                advance(s);

            s->lexer->result_symbol = s->m_LastToken = result;
            reset_active_modifiers(s);
            return true;
        }

        detached_modifier = is_detached_modifier(s->lexer->lookahead);
        ++i;
        ++s->m_ParsedChars;
    } while (detached_modifier);

    /* If we've only parsed one character and instantly failed then we might
     * be dealing with an attached modifier! */
    if (s->m_ParsedChars == 1)
    {
        const ModifierEntry* found_attached_modifier = find_attached_modifier(s->m_Current);
        if (found_attached_modifier != NULL
            && !s->m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2])
        {
            s->m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2] = true;
            s->lexer->result_symbol = s->m_LastToken = found_attached_modifier->second;
            return true;
        }
    }

    return false;
}

/**
 * Checks for the existence of an attached modifier
 */
static bool check_attached(Scanner* s)
{
    if (s->lexer->lookahead == ':')
    {
        bool is_current_char_whitespace = !s->m_Current || iswspace(s->m_Current);
        advance(s);

        if (is_current_char_whitespace || iswspace(s->lexer->lookahead))
            return false;

        s->lexer->result_symbol = s->m_LastToken = LINK_MODIFIER;
        return true;
    }

    if (s->lexer->lookahead == '|')
    {
        advance(s);

        const ModifierEntry* found_attached_modifier = find_attached_modifier(s->lexer->lookahead);

        if (s->m_LastToken >= BOLD_OPEN && s->m_LastToken <= INLINE_MACRO_CLOSE
            && (s->m_LastToken % 2) == (BOLD_OPEN % 2))
        {
            if (s->m_LastToken != VERBATIM_OPEN && s->m_LastToken != INLINE_MACRO_OPEN
                && s->m_LastToken != INLINE_MATH_OPEN && !can_have_modifier(s))
                return false;

            s->lexer->result_symbol = s->m_LastToken = FREE_FORM_MODIFIER_OPEN;
            return true;
        }
        else if (found_attached_modifier != NULL)
        {
            if (!can_have_modifier(s)
                && !(found_attached_modifier->second == VERBATIM_OPEN
                     && s->m_ActiveModifiers[(VERBATIM_OPEN - BOLD_OPEN) / 2])
                && !(found_attached_modifier->second == INLINE_MATH_OPEN
                     && s->m_ActiveModifiers[(INLINE_MATH_OPEN - BOLD_OPEN) / 2])
                && !(found_attached_modifier->second == INLINE_MACRO_OPEN
                     && s->m_ActiveModifiers[(INLINE_MACRO_OPEN - BOLD_OPEN) / 2]))
                return false;
            s->lexer->result_symbol = s->m_LastToken = FREE_FORM_MODIFIER_CLOSE;
            return true;
        }
        else
        {
            s->lexer->result_symbol = s->m_LastToken = WORD;
            return true;
        }
    }

    const ModifierEntry* found_attached_modifier = find_attached_modifier(s->lexer->lookahead);

    if (found_attached_modifier == NULL)
        return false;

    /* First check for the existence of an opening attached modifier */
    if (iswspace(s->m_Current)
        || (iswpunct(s->m_Current) && s->m_LastToken != FREE_FORM_MODIFIER_CLOSE)
        || !s->m_Current)
    {
        advance(s);

        /* empty attached modifier */
        if (s->lexer->lookahead == found_attached_modifier->first)
        {
            while (s->lexer->lookahead == found_attached_modifier->first)
                advance(s);
            return false;
        }

        if (!iswspace(s->lexer->lookahead)
            && !s->m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2]
            && can_have_modifier(s))
        {
            s->m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2] = true;
            s->lexer->result_symbol = s->m_LastToken = found_attached_modifier->second;
            return true;
        }
    }
    else
        advance(s);

    if (s->lexer->lookahead == found_attached_modifier->first)
    {
        while (s->lexer->lookahead == found_attached_modifier->first)
            advance(s);
        return false;
    }

    const ModifierEntry* found_next_attached_modifier = find_attached_modifier(s->lexer->lookahead);

    if (found_next_attached_modifier != NULL)
    {
        s->m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2] = false;
        s->lexer->result_symbol = s->m_LastToken =
            (TokenType)(found_attached_modifier->second + 1);
        return true;
    }

    if ((!iswspace(s->m_Previous) || !s->m_Previous)
        && (iswspace(s->lexer->lookahead) || iswpunct(s->lexer->lookahead) || !s->lexer->lookahead))
    {
        s->m_ActiveModifiers[(found_attached_modifier->second - BOLD_OPEN) / 2] = false;
        s->lexer->result_symbol = s->m_LastToken =
            (TokenType)(found_attached_modifier->second + 1);
        return true;
    }

    return false;
}

/// Attempts to parse a link location.
static bool check_link_location(Scanner* s)
{
    size_t count = 0;

    switch (s->m_LastToken) {
    case LINK_LOCATION_BEGIN:
        if (s->lexer->lookahead == ':')
        {
            s->lexer->result_symbol = s->m_LastToken = LINK_FILE_BEGIN;
            advance(s);
            return !iswspace(s->lexer->lookahead);
        }
        /* since we have no break here, if we do not detect a beginning of a
         * file segment we fall through into this next case statement */
        /* fallthrough */
    case INTERSECTING_MODIFIER:
    case LINK_FILE_END:
        switch (s->lexer->lookahead) {
        case '?':
            s->lexer->result_symbol = s->m_LastToken = LINK_TARGET_WIKI;
            break;
        case '#':
            s->lexer->result_symbol = s->m_LastToken = LINK_TARGET_GENERIC;
            break;
        case '/':
            if (s->m_LastToken == LINK_FILE_END)
                return false;
            s->lexer->result_symbol = s->m_LastToken = LINK_TARGET_EXTERNAL_FILE;
            break;
        case '@':
            if (s->m_LastToken == LINK_FILE_END)
                return false;
            s->lexer->result_symbol = s->m_LastToken = LINK_TARGET_TIMESTAMP;
            break;
        case '$':
            s->lexer->result_symbol = s->m_LastToken = LINK_TARGET_DEFINITION;
            break;
        case '^':
            s->lexer->result_symbol = s->m_LastToken = LINK_TARGET_FOOTNOTE;
            break;
        case '*':
            advance(s);

            while (s->lexer->lookahead == '*')
            {
                ++count;
                advance(s);
            }

            s->lexer->result_symbol = s->m_LastToken =
                (TokenType)(LINK_TARGET_HEADING1 + (count <= 5 ? count : 5));

            if (!iswspace(s->lexer->lookahead))
                return false;

            while (iswspace(s->lexer->lookahead))
                advance(s);

            return true;
        default:
            s->lexer->result_symbol = s->m_LastToken = iswdigit(s->lexer->lookahead) ?
                                                 LINK_TARGET_LINE_NUMBER : LINK_TARGET_URL;
            return true;
        }

        advance(s);

        if (!iswspace(s->lexer->lookahead))
            return false;

        while (iswspace(s->lexer->lookahead))
            advance(s);

        return true;
    case LINK_FILE_BEGIN:
        while (s->lexer->lookahead)
        {
            if (s->lexer->lookahead == ':' && s->m_Current != '\\')
                break;

            /* bail when potentially dealing with verbatim */
            if (s->lexer->lookahead == '`')
                return false;

            /* bail when potentially dealing with inline comments */
            if (s->lexer->lookahead == '%')
                return false;

            /* bail when potentially dealing with inline macro invocations */
            if (s->lexer->lookahead == '&')
                return false;

            /* bail when potentially dealing with inline math
             * Here we exclude the potential beginning of the file beginning
             * because this is how we denote links to other workspaces. */
            if (s->lexer->lookahead == '$' && s->m_Current != ':')
                return false;

            advance(s);
        }

        s->lexer->result_symbol = s->m_LastToken = LINK_FILE_TEXT;
        return true;
    case LINK_FILE_TEXT:
        if (s->lexer->lookahead == ':')
        {
            s->lexer->result_symbol = s->m_LastToken = LINK_FILE_END;
            advance(s);
            switch (s->lexer->lookahead) {
            case '}':
            case '#':
            case '%':
            case '$':
            case '^':
            case '*':
                return true;
            default:
                return iswdigit(s->lexer->lookahead);
            }
        }
        /* fallthrough */
    default:
        return false;
    }
}

/// Attempts to parse a detached modifier extension.
static bool check_detached_mod_extension(Scanner* s)
{
    switch (s->m_LastToken) {
    case DETACHED_MODIFIER_EXTENSION_BEGIN:
    case MODIFIER_EXTENSION_DELIMITER:
        switch (s->lexer->lookahead) {
        case '#':
            s->lexer->result_symbol = s->m_LastToken = PRIORITY;
            break;
        case '@':
            s->lexer->result_symbol = s->m_LastToken = TIMESTAMP;
            break;
        case ' ':
        case '\t':
        case '\v':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_UNDONE;
            break;
        case '-':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_PENDING;
            break;
        case 'x':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_DONE;
            break;
        case '=':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_ON_HOLD;
            break;
        case '_':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_CANCELLED;
            break;
        case '!':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_URGENT;
            break;
        case '?':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_UNCERTAIN;
            break;
        case '+':
            s->lexer->result_symbol = s->m_LastToken = TODO_ITEM_RECURRING;
            break;
        default:
            advance(s);
            return false;
        }

        advance(s);

        while (iswspace(s->lexer->lookahead))
            advance(s);

        return true;
    case TIMESTAMP:
    case PRIORITY:
    case TODO_ITEM_RECURRING:
        switch (s->lexer->lookahead) {
        case ')':
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = DETACHED_MODIFIER_EXTENSION_END;
            return true;
        case '|':
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = MODIFIER_EXTENSION_DELIMITER;
            return true;
        }

        while (s->lexer->lookahead && s->lexer->lookahead != '|' && s->lexer->lookahead != ')')
            advance(s);

        s->lexer->result_symbol = s->m_LastToken =
            (s->m_LastToken == TIMESTAMP || s->m_LastToken == TODO_ITEM_RECURRING) ?
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
        switch (s->lexer->lookahead) {
        case ')':
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = DETACHED_MODIFIER_EXTENSION_END;
            return true;
        case '|':
            if (find_attached_modifier(s->m_Current) == NULL)
            {
                advance(s);
                s->lexer->result_symbol = s->m_LastToken = MODIFIER_EXTENSION_DELIMITER;
                return true;
            }
        }
        return false;
    default:
        if (s->m_LastToken < HEADING1 || s->m_LastToken > MULTI_TABLE_CELL_SUFFIX)
            return false;

        switch (s->lexer->lookahead) {
        case '(':
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = DETACHED_MODIFIER_EXTENSION_BEGIN;
            return true;
        case ')':
            advance(s);
            s->lexer->result_symbol = s->m_LastToken = DETACHED_MODIFIER_EXTENSION_END;
            return true;
        }
    }

    return false;
}

/**
 * Simply parses any word (segment containing consecutive non-whitespace
 * characters). If in a tag parse_text parses till a newline is encountered.
 */
static bool parse_text(Scanner* s)
{
    if (s->m_TagContext == TAG_IN_VERBATIM_TAG)
    {
        while (!is_newline(s->lexer->lookahead))
            advance(s);
        s->lexer->result_symbol = s->m_LastToken = WORD;
        return true;
    }

    if (((char)s->m_TagContext % 2) == 0 && s->lexer->lookahead == '.')
    {
        advance(s);
        s->lexer->result_symbol = s->m_LastToken = TAG_DELIMITER;
        return true;
    }

    if (is_newline(s->lexer->lookahead))
    {
        s->lexer->result_symbol = s->m_LastToken = WORD;
        return true;
    }

    if (is_blank(s->lexer->lookahead))
    {
        do
            advance(s);
        while (is_blank(s->lexer->lookahead));

        if (s->lexer->lookahead == ':')
        {
            advance(s);
            if (is_blank(s->lexer->lookahead))
            {
                advance(s);
                s->lexer->result_symbol = s->m_LastToken = INTERSECTING_MODIFIER;
                return true;
            }
            else
            {
                s->lexer->result_symbol = s->m_LastToken = WORD;
                return true;
            }
        }

        s->lexer->result_symbol = s->m_LastToken = SPACE;
        return true;
    }

    const TokenType resulting_symbol = iswupper(s->lexer->lookahead) ?
                                       CAPITALIZED_WORD : WORD;

    do
    {
        bool _break = false;
        switch (s->lexer->lookahead) {
        case ':': case '|': case '~': case '\\':
        case '<': case '>':
        case '[': case ']':
        case '{': case '}':
        case '(': case ')':
            _break = true;
            break;
        default:
            break;
        }

        if (_break
            || find_attached_modifier(s->lexer->lookahead) != NULL
            || ((char)s->m_TagContext % 2 == 0 && s->lexer->lookahead == '.'))
            break;
        else
            advance(s);
    } while (s->lexer->lookahead && !iswspace(s->lexer->lookahead) && s->lexer->lookahead != '\\');

    s->lexer->result_symbol = s->m_LastToken = resulting_symbol;
    return true;
}

static void reset_active_modifiers(Scanner* s)
{
    memset(s->m_ActiveModifiers, 0, sizeof(s->m_ActiveModifiers));
}

static inline bool is_newline(int32_t c) { return !c || c == '\n' || c == '\r'; }

static inline bool is_blank(int32_t c) { return c && iswblank(c); }

/* ------------------------------------------------------------------------ */

void* tree_sitter_norg_external_scanner_create()
{
    Scanner* scanner = (Scanner*)malloc(sizeof(Scanner));

    /* Mirrors the C++ struct's default member initializers. */
    scanner->lexer = NULL;
    scanner->m_Previous = 0;
    scanner->m_Current = 0;
    scanner->m_TagContext = TAG_NONE;
    scanner->m_TagLevel = 0;
    scanner->m_InLinkLocation = false;
    scanner->m_LastToken = NONE;
    scanner->m_ParsedChars = 0;
    memset(scanner->m_ActiveModifiers, 0, sizeof(scanner->m_ActiveModifiers));

    return scanner;
}

void tree_sitter_norg_external_scanner_destroy(void* payload)
{
    free((Scanner*)payload);
}

bool tree_sitter_norg_external_scanner_scan(void* payload,
                                            TSLexer* lexer,
                                            const bool* valid_symbols)
{
    Scanner* scanner = (Scanner*)payload;
    scanner->lexer = lexer;
    return scan(scanner, valid_symbols);
}

unsigned tree_sitter_norg_external_scanner_serialize(void* payload, char* buffer)
{
    Scanner* scanner = (Scanner*)payload;

    if (8 + ACTIVE_MODIFIERS_SIZE >= TREE_SITTER_SERIALIZATION_BUFFER_SIZE)
        return 0;

    buffer[0] = (char)scanner->m_LastToken;
    buffer[1] = (char)scanner->m_TagLevel;
    buffer[2] = (char)scanner->m_TagContext;
    buffer[3] = (char)scanner->m_InLinkLocation;

    /* Copy 4 bytes from int32_t to buffer 2 to 6 positions. */
    memcpy(buffer + 4, &scanner->m_Current, 4);

    /* Serialize the attached modifier bit array into the char array. */
    for (int i = 0; i < ACTIVE_MODIFIERS_SIZE; ++i)
        buffer[8 + i] = (char)scanner->m_ActiveModifiers[i];

    return 8 + ACTIVE_MODIFIERS_SIZE;
}

void tree_sitter_norg_external_scanner_deserialize(void* payload,
                                                   const char* buffer,
                                                   unsigned length)
{
    Scanner* scanner = (Scanner*)payload;

    if (length == 0)
    {
        scanner->m_TagLevel = 0;
        scanner->m_TagContext = TAG_NONE;
        scanner->m_InLinkLocation = false;
        scanner->m_LastToken = NONE;
        scanner->m_Current = 0;
        memset(scanner->m_ActiveModifiers, 0, sizeof(scanner->m_ActiveModifiers));
        return;
    }

    scanner->m_LastToken = (TokenType)buffer[0];
    scanner->m_TagLevel = (size_t)buffer[1];
    scanner->m_TagContext = (TagType)buffer[2];
    scanner->m_InLinkLocation = (bool)buffer[3];

    memcpy(&scanner->m_Current, buffer + 4, 4);

    for (int i = 0; i < ACTIVE_MODIFIERS_SIZE; ++i)
        scanner->m_ActiveModifiers[i] = (bool)buffer[8 + i];
}
