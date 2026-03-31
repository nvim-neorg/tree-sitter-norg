#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <wctype.h>

#include "tree_sitter/parser.h"

enum TokenType
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

enum TagType
{
    TAG_NONE = 1,
    TAG_ON_REGULAR = 2,
    TAG_IN_REGULAR = 3,
    TAG_ON_VERBATIM = 4,
    TAG_IN_VERBATIM = 5,
};

#define IS_TAG_ON_LINE(ctx) ((ctx) != TAG_NONE && ((ctx) % 2) == 0)
#define MODIFIER_INDEX(tok) (((tok) - BOLD_OPEN) / 2)
#define MODIFIER_COUNT (MODIFIER_INDEX(INLINE_MACRO_OPEN) + 1)

typedef struct
{
    TSLexer* lexer;
    int32_t previous;
    int32_t current;
    enum TagType tag_context;
    size_t tag_level;
    bool in_link_location;
    enum TokenType last_token;
    size_t parsed_chars;
    bool active_modifiers[MODIFIER_COUNT];
} Scanner;

#define EMIT(s, tok)                                         \
    do                                                       \
    {                                                        \
        (s)->lexer->result_symbol = (s)->last_token = (tok); \
        return true;                                         \
    } while (0)

static bool is_newline(int32_t c)
{
    return !c || c == '\n' || c == '\r';
}
static bool is_blank(int32_t c)
{
    return c && iswblank(c);
}

static bool is_detached_modifier_char(int32_t c)
{
    switch (c)
    {
    case '*':
    case '-':
    case '>':
    case '%':
    case '=':
    case '~':
    case '$':
    case '_':
    case '^':
    case '&':
    case '<':
    case ':':
        return true;
    default:
        return false;
    }
}

static bool look_up_attached_mod(int32_t c, enum TokenType* out)
{
    switch (c)
    {
    case '*':
        *out = BOLD_OPEN;
        return true;
    case '/':
        *out = ITALIC_OPEN;
        return true;
    case '-':
        *out = STRIKETHROUGH_OPEN;
        return true;
    case '_':
        *out = UNDERLINE_OPEN;
        return true;
    case '!':
        *out = SPOILER_OPEN;
        return true;
    case '`':
        *out = VERBATIM_OPEN;
        return true;
    case '^':
        *out = SUPERSCRIPT_OPEN;
        return true;
    case ',':
        *out = SUBSCRIPT_OPEN;
        return true;
    case '%':
        *out = INLINE_COMMENT_OPEN;
        return true;
    case '$':
        *out = INLINE_MATH_OPEN;
        return true;
    case '&':
        *out = INLINE_MACRO_OPEN;
        return true;
    default:
        return false;
    }
}

static bool is_attached_modifier_char(int32_t c)
{
    enum TokenType unused;
    return look_up_attached_mod(c, &unused);
}

static void skip(Scanner* s)
{
    s->previous = s->current;
    s->current = s->lexer->lookahead;
    s->lexer->advance(s->lexer, true);
}

static void advance(Scanner* s)
{
    s->previous = s->current;
    s->current = s->lexer->lookahead;
    s->lexer->advance(s->lexer, false);
}

// used only for matching the "end" keyword in tags
static bool match_keyword(Scanner* s, const char* str)
{
    for (const char* p = str; *p; p++)
    {
        if ((int32_t)*p != s->lexer->lookahead)
            return false;
        advance(s);
    }
    return true;
}

static void reset_active_modifiers(Scanner* s)
{
    memset(s->active_modifiers, 0, sizeof(s->active_modifiers));
}

static bool can_open_modifier(const Scanner* s)
{
    return !s->active_modifiers[MODIFIER_INDEX(VERBATIM_OPEN)] &&
           !s->active_modifiers[MODIFIER_INDEX(INLINE_MATH_OPEN)] &&
           !s->active_modifiers[MODIFIER_INDEX(INLINE_MACRO_OPEN)];
}

static bool parse_text(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    if (s->tag_context == TAG_IN_VERBATIM)
    {
        while (!is_newline(lexer->lookahead))
            advance(s);
        EMIT(s, WORD);
    }

    if (IS_TAG_ON_LINE(s->tag_context) && lexer->lookahead == '.')
    {
        advance(s);
        EMIT(s, TAG_DELIMITER);
    }

    if (is_newline(lexer->lookahead))
        EMIT(s, WORD);

    if (is_blank(lexer->lookahead))
    {
        do
            advance(s);
        while (is_blank(lexer->lookahead));

        if (lexer->lookahead == ':')
        {
            advance(s);
            if (is_blank(lexer->lookahead))
            {
                advance(s);
                EMIT(s, INTERSECTING_MODIFIER);
            }
            EMIT(s, WORD);
        }

        EMIT(s, SPACE);
    }

    enum TokenType result = iswupper(lexer->lookahead) ? CAPITALIZED_WORD : WORD;

    do
    {
        bool is_special = false;
        switch (lexer->lookahead)
        {
        case ':':
        case '|':
        case '~':
        case '\\':
        case '<':
        case '>':
        case '[':
        case ']':
        case '{':
        case '}':
        case '(':
        case ')':
            is_special = true;
            break;
        default:
            break;
        }

        if (is_special || is_attached_modifier_char(lexer->lookahead) ||
            (IS_TAG_ON_LINE(s->tag_context) && lexer->lookahead == '.'))
            break;

        advance(s);
    } while (lexer->lookahead && !iswspace(lexer->lookahead) && lexer->lookahead != '\\');

    EMIT(s, result);
}

static bool check_detached(Scanner* s,
                           const enum TokenType* results,
                           size_t count,
                           int32_t expected)
{
    TSLexer* lexer = s->lexer;
    size_t level = 0;
    s->parsed_chars = 0;

    do
    {
        if (lexer->lookahead != expected)
            break;

        advance(s);

        if (is_blank(lexer->lookahead))
        {
            size_t max_level = count - 1;
            enum TokenType token = results[level <= max_level ? level : max_level];

            while (is_blank(lexer->lookahead))
                advance(s);

            lexer->result_symbol = s->last_token = token;
            reset_active_modifiers(s);
            return true;
        }

        ++level;
        ++s->parsed_chars;
    } while (is_detached_modifier_char(lexer->lookahead));

    if (s->parsed_chars == 1)
    {
        enum TokenType mod;
        if (look_up_attached_mod(s->current, &mod) && !s->active_modifiers[MODIFIER_INDEX(mod)])
        {
            s->active_modifiers[MODIFIER_INDEX(mod)] = true;
            EMIT(s, mod);
        }
    }

    return false;
}

static bool check_detached_or_suffix(Scanner* s,
                                     const enum TokenType* results,
                                     size_t count,
                                     int32_t expected,
                                     size_t required_chars,
                                     enum TokenType suffix,
                                     bool advance_past_newline)
{
    TSLexer* lexer = s->lexer;

    if (check_detached(s, results, count, expected))
        return true;

    if (is_newline(lexer->lookahead) && s->parsed_chars >= required_chars)
    {
        if (advance_past_newline)
            advance(s);
        EMIT(s, suffix);
    }

    return false;
}

static bool scan_verbatim_tag(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    advance(s);
    lexer->mark_end(lexer);

    if (match_keyword(s, "end") && (iswspace(lexer->lookahead) || !lexer->lookahead))
    {
        while (is_blank(lexer->lookahead))
            advance(s);

        if ((iswspace(lexer->lookahead) || !lexer->lookahead) && s->tag_context == TAG_IN_VERBATIM)
        {
            s->tag_context = TAG_NONE;
            EMIT(s, RANGED_VERBATIM_TAG_END);
        }

        EMIT(s, WORD);
    }

    if (s->last_token == RANGED_VERBATIM_TAG || s->tag_context == TAG_IN_VERBATIM)
        EMIT(s, WORD);

    s->tag_context = TAG_ON_VERBATIM;
    EMIT(s, RANGED_VERBATIM_TAG);
}

static bool scan_macro_tag(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    advance(s);
    lexer->mark_end(lexer);

    if (match_keyword(s, "end") && (iswspace(lexer->lookahead) || !lexer->lookahead))
    {
        while (lexer->lookahead && iswspace(lexer->lookahead) && !is_newline(lexer->lookahead))
            advance(s);

        if ((iswspace(lexer->lookahead) || !lexer->lookahead) && s->tag_level)
        {
            --s->tag_level;
            EMIT(s, MACRO_TAG_END);
        }

        EMIT(s, WORD);
    }

    if (lexer->lookahead == '=')
    {
        advance(s);
        if (lexer->lookahead == '=')
        {
            do
                advance(s);
            while (lexer->lookahead == '=');

            lexer->mark_end(lexer);
            advance(s);
            EMIT(s, is_newline(s->current) ? STRONG_PARAGRAPH_DELIMITER : WORD);
        }
        lexer->mark_end(lexer);
        EMIT(s, WORD);
    }

    if (s->last_token == MACRO_TAG)
        EMIT(s, WORD);

    s->tag_context = TAG_ON_REGULAR;
    ++s->tag_level;
    EMIT(s, MACRO_TAG);
}

static bool scan_ranged_tag(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    advance(s);
    lexer->mark_end(lexer);

    if (match_keyword(s, "end") && (iswspace(lexer->lookahead) || !lexer->lookahead))
    {
        while (is_blank(lexer->lookahead))
            advance(s);

        if ((iswspace(lexer->lookahead) || !lexer->lookahead) && s->tag_level)
        {
            --s->tag_level;
            EMIT(s, RANGED_TAG_END);
        }

        EMIT(s, WORD);
    }

    if (s->last_token == RANGED_TAG)
        EMIT(s, WORD);

    s->tag_context = TAG_ON_REGULAR;
    ++s->tag_level;
    EMIT(s, RANGED_TAG);
}

static bool check_detached_mod_extension(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    switch (s->last_token)
    {
    case DETACHED_MODIFIER_EXTENSION_BEGIN:
    case MODIFIER_EXTENSION_DELIMITER:
    {
        enum TokenType tok;
        switch (lexer->lookahead)
        {
        case '#':
            tok = PRIORITY;
            break;
        case '@':
            tok = TIMESTAMP;
            break;
        case ' ':
        case '\t':
        case '\v':
            tok = TODO_ITEM_UNDONE;
            break;
        case '-':
            tok = TODO_ITEM_PENDING;
            break;
        case 'x':
            tok = TODO_ITEM_DONE;
            break;
        case '=':
            tok = TODO_ITEM_ON_HOLD;
            break;
        case '_':
            tok = TODO_ITEM_CANCELLED;
            break;
        case '!':
            tok = TODO_ITEM_URGENT;
            break;
        case '?':
            tok = TODO_ITEM_UNCERTAIN;
            break;
        case '+':
            tok = TODO_ITEM_RECURRING;
            break;
        default:
            advance(s);
            return false;
        }

        advance(s);
        while (iswspace(lexer->lookahead))
            advance(s);
        EMIT(s, tok);
    }

    case TIMESTAMP:
    case PRIORITY:
    case TODO_ITEM_RECURRING:
        if (lexer->lookahead == ')')
        {
            advance(s);
            EMIT(s, DETACHED_MODIFIER_EXTENSION_END);
        }
        if (lexer->lookahead == '|')
        {
            advance(s);
            EMIT(s, MODIFIER_EXTENSION_DELIMITER);
        }

        while (lexer->lookahead && lexer->lookahead != '|' && lexer->lookahead != ')')
            advance(s);

        EMIT(s, (s->last_token == TIMESTAMP || s->last_token == TODO_ITEM_RECURRING)
                    ? TIMESTAMP_DATA
                    : PRIORITY_DATA);

    case TODO_ITEM_UNDONE:
    case TODO_ITEM_PENDING:
    case TODO_ITEM_DONE:
    case TODO_ITEM_ON_HOLD:
    case TODO_ITEM_CANCELLED:
    case TODO_ITEM_URGENT:
    case TODO_ITEM_UNCERTAIN:
    case TIMESTAMP_DATA:
    case PRIORITY_DATA:
        if (lexer->lookahead == ')')
        {
            advance(s);
            EMIT(s, DETACHED_MODIFIER_EXTENSION_END);
        }
        if (lexer->lookahead == '|' && !is_attached_modifier_char(s->current))
        {
            advance(s);
            EMIT(s, MODIFIER_EXTENSION_DELIMITER);
        }
        return false;

    default:
        if (s->last_token < HEADING1 || s->last_token > MULTI_TABLE_CELL_SUFFIX)
            return false;
        if (lexer->lookahead == '(')
        {
            advance(s);
            EMIT(s, DETACHED_MODIFIER_EXTENSION_BEGIN);
        }
        if (lexer->lookahead == ')')
        {
            advance(s);
            EMIT(s, DETACHED_MODIFIER_EXTENSION_END);
        }
        return false;
    }
}

static bool check_link_location(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    switch (s->last_token)
    {
    case LINK_LOCATION_BEGIN:
        if (lexer->lookahead == ':')
        {
            lexer->result_symbol = s->last_token = LINK_FILE_BEGIN;
            advance(s);
            return !iswspace(lexer->lookahead);
        }
        /* fall through */

    case INTERSECTING_MODIFIER:
    case LINK_FILE_END:
    {
        enum TokenType target;
        switch (lexer->lookahead)
        {
        case '?':
            target = LINK_TARGET_WIKI;
            break;
        case '#':
            target = LINK_TARGET_GENERIC;
            break;
        case '/':
            if (s->last_token == LINK_FILE_END)
                return false;
            target = LINK_TARGET_EXTERNAL_FILE;
            break;
        case '@':
            if (s->last_token == LINK_FILE_END)
                return false;
            target = LINK_TARGET_TIMESTAMP;
            break;
        case '$':
            target = LINK_TARGET_DEFINITION;
            break;
        case '^':
            target = LINK_TARGET_FOOTNOTE;
            break;
        case '*':
        {
            advance(s);
            size_t extra = 0;
            while (lexer->lookahead == '*')
            {
                ++extra;
                advance(s);
            }

            lexer->result_symbol = s->last_token =
                (enum TokenType)(LINK_TARGET_HEADING1 + (extra <= 5 ? extra : 5));

            if (!iswspace(lexer->lookahead))
                return false;
            while (iswspace(lexer->lookahead))
                advance(s);
            return true;
        }
        default:
            EMIT(s, iswdigit(lexer->lookahead) ? LINK_TARGET_LINE_NUMBER : LINK_TARGET_URL);
        }

        advance(s);
        if (!iswspace(lexer->lookahead))
            return false;
        while (iswspace(lexer->lookahead))
            advance(s);
        EMIT(s, target);
    }

    case LINK_FILE_BEGIN:
        while (lexer->lookahead)
        {
            if (lexer->lookahead == ':' && s->current != '\\')
                break;
            if (lexer->lookahead == '`' || lexer->lookahead == '%' || lexer->lookahead == '&')
                return false;
            if (lexer->lookahead == '$' && s->current != ':')
                return false;
            advance(s);
        }
        EMIT(s, LINK_FILE_TEXT);

    case LINK_FILE_TEXT:
        if (lexer->lookahead == ':')
        {
            lexer->result_symbol = s->last_token = LINK_FILE_END;
            advance(s);
            switch (lexer->lookahead)
            {
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
        /* fall through */

    default:
        return false;
    }
}

static bool check_attached(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    if (lexer->lookahead == ':')
    {
        bool prev_is_ws = !s->current || iswspace(s->current);
        advance(s);
        if (prev_is_ws || iswspace(lexer->lookahead))
            return false;
        EMIT(s, LINK_MODIFIER);
    }

    if (lexer->lookahead == '|')
    {
        advance(s);

        enum TokenType next_mod;
        bool next_is_mod = look_up_attached_mod(lexer->lookahead, &next_mod);

        if (s->last_token >= BOLD_OPEN && s->last_token <= INLINE_MACRO_CLOSE &&
            (s->last_token % 2) == (BOLD_OPEN % 2))
        {
            if (s->last_token != VERBATIM_OPEN && s->last_token != INLINE_MACRO_OPEN &&
                s->last_token != INLINE_MATH_OPEN && !can_open_modifier(s))
                return false;
            EMIT(s, FREE_FORM_MODIFIER_OPEN);
        }

        if (next_is_mod)
        {
            if (!can_open_modifier(s) &&
                !(next_mod == VERBATIM_OPEN &&
                  s->active_modifiers[MODIFIER_INDEX(VERBATIM_OPEN)]) &&
                !(next_mod == INLINE_MATH_OPEN &&
                  s->active_modifiers[MODIFIER_INDEX(INLINE_MATH_OPEN)]) &&
                !(next_mod == INLINE_MACRO_OPEN &&
                  s->active_modifiers[MODIFIER_INDEX(INLINE_MACRO_OPEN)]))
                return false;
            EMIT(s, FREE_FORM_MODIFIER_CLOSE);
        }

        EMIT(s, WORD);
    }

    enum TokenType mod;
    if (!look_up_attached_mod(lexer->lookahead, &mod))
        return false;

    int32_t mod_char = lexer->lookahead;

    if (iswspace(s->current) ||
        (iswpunct(s->current) && s->last_token != FREE_FORM_MODIFIER_CLOSE) || !s->current)
    {
        advance(s);

        if (lexer->lookahead == mod_char)
        {
            while (lexer->lookahead == mod_char)
                advance(s);
            return false;
        }

        if (!iswspace(lexer->lookahead) && !s->active_modifiers[MODIFIER_INDEX(mod)] &&
            can_open_modifier(s))
        {
            s->active_modifiers[MODIFIER_INDEX(mod)] = true;
            EMIT(s, mod);
        }
    }
    else
    {
        advance(s);
    }

    if (lexer->lookahead == mod_char)
    {
        while (lexer->lookahead == mod_char)
            advance(s);
        return false;
    }

    enum TokenType close_tok = (enum TokenType)(mod + 1);

    enum TokenType next_mod;
    if (look_up_attached_mod(lexer->lookahead, &next_mod))
    {
        s->active_modifiers[MODIFIER_INDEX(mod)] = false;
        EMIT(s, close_tok);
    }

    if ((!iswspace(s->previous) || !s->previous) &&
        (iswspace(lexer->lookahead) || iswpunct(lexer->lookahead) || !lexer->lookahead))
    {
        s->active_modifiers[MODIFIER_INDEX(mod)] = false;
        EMIT(s, close_tok);
    }

    return false;
}

static bool scan_punctuation(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    switch (lexer->lookahead)
    {
    case '<':
        advance(s);
        if (!iswspace(lexer->lookahead))
        {
            s->in_link_location = true;
            EMIT(s, INLINE_LINK_TARGET_OPEN);
        }
        break;

    case '>':
        advance(s);
        if (!iswspace(s->previous) && s->last_token != LINK_LOCATION_BEGIN &&
            s->last_token != LINK_FILE_END)
        {
            s->in_link_location = false;
            EMIT(s, INLINE_LINK_TARGET_CLOSE);
        }
        break;

    case '(':
        advance(s);
        if (!iswspace(lexer->lookahead) && s->last_token != NONE &&
            ((s->last_token >= BOLD_OPEN && s->last_token <= INLINE_MACRO_CLOSE &&
              (s->last_token % 2) == (BOLD_CLOSE % 2)) ||
             s->last_token == LINK_DESCRIPTION_END || s->last_token == LINK_LOCATION_END ||
             s->last_token == INLINE_LINK_TARGET_CLOSE))
        {
            EMIT(s, ATTACHED_MODIFIER_BEGIN);
        }
        EMIT(s, WORD);

    case ')':
        advance(s);
        if (!iswspace(s->previous))
            EMIT(s, ATTACHED_MODIFIER_END);
        break;

    case '[':
        advance(s);
        if (!iswspace(lexer->lookahead))
            EMIT(s, LINK_DESCRIPTION_BEGIN);
        break;

    case ']':
        advance(s);
        if (!iswspace(s->previous))
            EMIT(s, LINK_DESCRIPTION_END);
        break;

    case '{':
        advance(s);
        if (!iswspace(lexer->lookahead))
        {
            s->in_link_location = true;
            EMIT(s, LINK_LOCATION_BEGIN);
        }
        break;

    case '}':
        advance(s);
        if (is_newline(s->previous))
            EMIT(s, NONE);
        if (!iswspace(s->previous))
        {
            s->in_link_location = false;
            EMIT(s, LINK_LOCATION_END);
        }
        break;
    }

    return false;
}

static bool scan_newline(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    advance(s);
    lexer->result_symbol = s->last_token = LINE_BREAK;

    if (lexer->eof(lexer) || !lexer->lookahead)
    {
        reset_active_modifiers(s);
        return true;
    }

    if (IS_TAG_ON_LINE(s->tag_context))
    {
        s->tag_context = (enum TagType)((char)s->tag_context + 1);
        return true;
    }

    if (is_newline(lexer->lookahead))
    {
        advance(s);
        lexer->result_symbol = s->last_token = PARAGRAPH_BREAK;
        reset_active_modifiers(s);
    }

    return true;
}

static bool scan_line_start(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    while (is_blank(lexer->lookahead))
        skip(s);

    if (lexer->lookahead == '@')
        return scan_verbatim_tag(s);

    if (s->tag_context == TAG_IN_VERBATIM)
        return parse_text(s);

    if (lexer->lookahead == '=')
        return scan_macro_tag(s);

    if (lexer->lookahead == '|')
        return scan_ranged_tag(s);

    if (lexer->lookahead == '#')
    {
        advance(s);
        if (!lexer->lookahead || iswspace(lexer->lookahead))
            EMIT(s, is_newline(lexer->lookahead) ? INDENT_SEGMENT : WORD);
        EMIT(s, STRONG_CARRYOVER);
    }

    if (lexer->lookahead == '+')
    {
        advance(s);
        if (lexer->lookahead != '+')
            EMIT(s, WEAK_CARRYOVER);
    }

    if (lexer->lookahead == '.')
    {
        advance(s);
        if (lexer->lookahead != '.')
            EMIT(s, INFIRM_TAG);
    }

    {
        const enum TokenType toks[] = {HEADING1, HEADING2, HEADING3, HEADING4, HEADING5, HEADING6};
        if (check_detached(s, toks, 6, '*'))
            return true;
    }

    {
        const enum TokenType toks[] = {QUOTE1, QUOTE2, QUOTE3, QUOTE4, QUOTE5, QUOTE6};
        if (check_detached(s, toks, 6, '>'))
            return true;
    }

    {
        const enum TokenType toks[] = {UNORDERED_LIST1, UNORDERED_LIST2, UNORDERED_LIST3,
                                       UNORDERED_LIST4, UNORDERED_LIST5, UNORDERED_LIST6};
        if (check_detached_or_suffix(s, toks, 6, '-', 3, WEAK_PARAGRAPH_DELIMITER, true))
            return true;
    }

    {
        const enum TokenType toks[] = {ORDERED_LIST1, ORDERED_LIST2, ORDERED_LIST3,
                                       ORDERED_LIST4, ORDERED_LIST5, ORDERED_LIST6};
        if (check_detached(s, toks, 6, '~'))
            return true;
        if (is_newline(lexer->lookahead) && s->parsed_chars == 1)
        {
            if (lexer->eof(lexer))
            {
                reset_active_modifiers(s);
                return false;
            }
            EMIT(s, TRAILING_MODIFIER);
        }
    }

    {
        const enum TokenType toks[] = {SINGLE_DEFINITION, MULTI_DEFINITION, NONE};
        if (check_detached_or_suffix(s, toks, 3, '$', 2, MULTI_DEFINITION_SUFFIX, true))
            return true;
    }

    {
        const enum TokenType toks[] = {SINGLE_FOOTNOTE, MULTI_FOOTNOTE, NONE};
        if (check_detached_or_suffix(s, toks, 3, '^', 2, MULTI_FOOTNOTE_SUFFIX, true))
            return true;
    }

    {
        const enum TokenType toks[] = {SINGLE_TABLE_CELL, MULTI_TABLE_CELL, NONE};
        if (check_detached_or_suffix(s, toks, 3, ':', 2, MULTI_TABLE_CELL_SUFFIX, true))
            return true;
    }

    {
        const enum TokenType toks[] = {NONE, NONE};
        if (check_detached_or_suffix(s, toks, 2, '_', 3, HORIZONTAL_LINE, false))
            return true;
    }

    return false;
}

static bool scan_slide_or_indent(Scanner* s)
{
    TSLexer* lexer = s->lexer;

    if (!((s->last_token >= HEADING1 && s->last_token <= MULTI_TABLE_CELL_SUFFIX) ||
          s->last_token == DETACHED_MODIFIER_EXTENSION_END))
        return false;

    if (lexer->lookahead != ':')
        return false;

    advance(s);
    bool is_indent = false;

    if (lexer->lookahead == ':')
    {
        advance(s);
        is_indent = true;
    }

    if (!is_newline(lexer->lookahead))
        EMIT(s, WORD);

    advance(s);
    EMIT(s, (enum TokenType)(SLIDE + is_indent));
}

static bool scanner_scan(Scanner* s, const bool* valid_symbols)
{
    TSLexer* lexer = s->lexer;
    lexer->result_symbol = NONE;

    if (lexer->eof(lexer) || !lexer->lookahead)
    {
        reset_active_modifiers(s);
        return false;
    }

    if (s->last_token == TRAILING_MODIFIER)
    {
        advance(s);
        return parse_text(s);
    }

    if (is_newline(lexer->lookahead))
        return scan_newline(s);

    if (lexer->get_column(lexer) == 0 && scan_line_start(s))
        return true;

    if (lexer->lookahead == '~')
    {
        advance(s);
        lexer->mark_end(lexer);
        if (is_newline(lexer->lookahead))
        {
            advance(s);
            if (lexer->eof(lexer))
            {
                reset_active_modifiers(s);
                return false;
            }
            EMIT(s, TRAILING_MODIFIER);
        }
        return parse_text(s);
    }

    if (lexer->lookahead == '\\')
    {
        advance(s);
        EMIT(s, ESCAPE_SEQUENCE);
    }

    if (check_detached_mod_extension(s))
        return true;

    if (scan_slide_or_indent(s))
        return true;

    if (scan_punctuation(s))
        return true;

    if (s->in_link_location && check_link_location(s))
        return true;

    if (check_attached(s))
        return true;

    return parse_text(s);
}

void* tree_sitter_norg_external_scanner_create(void)
{
    Scanner* s = (Scanner*)calloc(1, sizeof(Scanner));
    s->tag_context = TAG_NONE;
    return s;
}

void tree_sitter_norg_external_scanner_destroy(void* payload)
{
    free(payload);
}

bool tree_sitter_norg_external_scanner_scan(void* payload,
                                            TSLexer* lexer,
                                            const bool* valid_symbols)
{
    Scanner* s = (Scanner*)payload;
    s->lexer = lexer;
    return scanner_scan(s, valid_symbols);
}

unsigned tree_sitter_norg_external_scanner_serialize(void* payload, char* buffer)
{
    Scanner* s = (Scanner*)payload;

    if (8 + MODIFIER_COUNT >= TREE_SITTER_SERIALIZATION_BUFFER_SIZE)
        return 0;

    buffer[0] = (char)s->last_token;
    buffer[1] = (char)s->tag_level;
    buffer[2] = (char)s->tag_context;
    buffer[3] = (char)s->in_link_location;
    memcpy(buffer + 4, &s->current, 4);

    for (int i = 0; i < MODIFIER_COUNT; ++i)
        buffer[8 + i] = s->active_modifiers[i];

    return 8 + MODIFIER_COUNT;
}

void tree_sitter_norg_external_scanner_deserialize(void* payload,
                                                   const char* buffer,
                                                   unsigned length)
{
    Scanner* s = (Scanner*)payload;

    if (length == 0)
    {
        s->tag_level = 0;
        s->tag_context = TAG_NONE;
        s->in_link_location = false;
        s->last_token = NONE;
        s->current = 0;
        memset(s->active_modifiers, 0, sizeof(s->active_modifiers));
        return;
    }

    s->last_token = (enum TokenType)buffer[0];
    s->tag_level = (size_t)(unsigned char)buffer[1];
    s->tag_context = (enum TagType)buffer[2];
    s->in_link_location = (bool)buffer[3];
    memcpy(&s->current, buffer + 4, 4);

    for (int i = 0; i < MODIFIER_COUNT; ++i)
        s->active_modifiers[i] = buffer[8 + i];
}
