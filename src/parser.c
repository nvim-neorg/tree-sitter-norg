#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 13
#define STATE_COUNT 60
#define LARGE_STATE_COUNT 7
#define SYMBOL_COUNT 41
#define ALIAS_COUNT 0
#define TOKEN_COUNT 22
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 4
#define PRODUCTION_ID_COUNT 1

enum {
  aux_sym_quote_token1 = 1,
  aux_sym_heading1_token1 = 2,
  aux_sym_heading2_token1 = 3,
  aux_sym_heading3_token1 = 4,
  aux_sym_heading4_token1 = 5,
  aux_sym_unordered_list_token1 = 6,
  aux_sym_marker_token1 = 7,
  aux_sym_todo_item_done_token1 = 8,
  aux_sym_todo_item_pending_token1 = 9,
  aux_sym_todo_item_undone_token1 = 10,
  sym__eof = 11,
  anon_sym_LF = 12,
  anon_sym_STAR = 13,
  anon_sym__ = 14,
  anon_sym_BQUOTE = 15,
  anon_sym_PIPE = 16,
  anon_sym_CARET = 17,
  anon_sym_BSLASH = 18,
  aux_sym_words_token1 = 19,
  aux_sym_words_token2 = 20,
  sym_trailing_modifier = 21,
  sym_document = 22,
  sym_quote = 23,
  sym_heading1 = 24,
  sym_heading2 = 25,
  sym_heading3 = 26,
  sym_heading4 = 27,
  sym_unordered_list = 28,
  sym_marker = 29,
  sym_todo_item_done = 30,
  sym_todo_item_pending = 31,
  sym_todo_item_undone = 32,
  sym_paragraph = 33,
  sym__soft_paragraph_break = 34,
  sym_words = 35,
  sym_paragraph_segment = 36,
  aux_sym_document_repeat1 = 37,
  aux_sym_paragraph_repeat1 = 38,
  aux_sym_paragraph_repeat2 = 39,
  aux_sym_words_repeat1 = 40,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [aux_sym_quote_token1] = "quote_token1",
  [aux_sym_heading1_token1] = "heading1_token1",
  [aux_sym_heading2_token1] = "heading2_token1",
  [aux_sym_heading3_token1] = "heading3_token1",
  [aux_sym_heading4_token1] = "heading4_token1",
  [aux_sym_unordered_list_token1] = "unordered_list_token1",
  [aux_sym_marker_token1] = "marker_token1",
  [aux_sym_todo_item_done_token1] = "todo_item_done_token1",
  [aux_sym_todo_item_pending_token1] = "todo_item_pending_token1",
  [aux_sym_todo_item_undone_token1] = "todo_item_undone_token1",
  [sym__eof] = "_eof",
  [anon_sym_LF] = "\n",
  [anon_sym_STAR] = "*",
  [anon_sym__] = "_",
  [anon_sym_BQUOTE] = "`",
  [anon_sym_PIPE] = "|",
  [anon_sym_CARET] = "^",
  [anon_sym_BSLASH] = "\\",
  [aux_sym_words_token1] = "words_token1",
  [aux_sym_words_token2] = "words_token2",
  [sym_trailing_modifier] = "trailing_modifier",
  [sym_document] = "document",
  [sym_quote] = "quote",
  [sym_heading1] = "heading1",
  [sym_heading2] = "heading2",
  [sym_heading3] = "heading3",
  [sym_heading4] = "heading4",
  [sym_unordered_list] = "unordered_list",
  [sym_marker] = "marker",
  [sym_todo_item_done] = "todo_item_done",
  [sym_todo_item_pending] = "todo_item_pending",
  [sym_todo_item_undone] = "todo_item_undone",
  [sym_paragraph] = "paragraph",
  [sym__soft_paragraph_break] = "_soft_paragraph_break",
  [sym_words] = "words",
  [sym_paragraph_segment] = "paragraph_segment",
  [aux_sym_document_repeat1] = "document_repeat1",
  [aux_sym_paragraph_repeat1] = "paragraph_repeat1",
  [aux_sym_paragraph_repeat2] = "paragraph_repeat2",
  [aux_sym_words_repeat1] = "words_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [aux_sym_quote_token1] = aux_sym_quote_token1,
  [aux_sym_heading1_token1] = aux_sym_heading1_token1,
  [aux_sym_heading2_token1] = aux_sym_heading2_token1,
  [aux_sym_heading3_token1] = aux_sym_heading3_token1,
  [aux_sym_heading4_token1] = aux_sym_heading4_token1,
  [aux_sym_unordered_list_token1] = aux_sym_unordered_list_token1,
  [aux_sym_marker_token1] = aux_sym_marker_token1,
  [aux_sym_todo_item_done_token1] = aux_sym_todo_item_done_token1,
  [aux_sym_todo_item_pending_token1] = aux_sym_todo_item_pending_token1,
  [aux_sym_todo_item_undone_token1] = aux_sym_todo_item_undone_token1,
  [sym__eof] = sym__eof,
  [anon_sym_LF] = anon_sym_LF,
  [anon_sym_STAR] = anon_sym_STAR,
  [anon_sym__] = anon_sym__,
  [anon_sym_BQUOTE] = anon_sym_BQUOTE,
  [anon_sym_PIPE] = anon_sym_PIPE,
  [anon_sym_CARET] = anon_sym_CARET,
  [anon_sym_BSLASH] = anon_sym_BSLASH,
  [aux_sym_words_token1] = aux_sym_words_token1,
  [aux_sym_words_token2] = aux_sym_words_token2,
  [sym_trailing_modifier] = sym_trailing_modifier,
  [sym_document] = sym_document,
  [sym_quote] = sym_quote,
  [sym_heading1] = sym_heading1,
  [sym_heading2] = sym_heading2,
  [sym_heading3] = sym_heading3,
  [sym_heading4] = sym_heading4,
  [sym_unordered_list] = sym_unordered_list,
  [sym_marker] = sym_marker,
  [sym_todo_item_done] = sym_todo_item_done,
  [sym_todo_item_pending] = sym_todo_item_pending,
  [sym_todo_item_undone] = sym_todo_item_undone,
  [sym_paragraph] = sym_paragraph,
  [sym__soft_paragraph_break] = sym__soft_paragraph_break,
  [sym_words] = sym_words,
  [sym_paragraph_segment] = sym_paragraph_segment,
  [aux_sym_document_repeat1] = aux_sym_document_repeat1,
  [aux_sym_paragraph_repeat1] = aux_sym_paragraph_repeat1,
  [aux_sym_paragraph_repeat2] = aux_sym_paragraph_repeat2,
  [aux_sym_words_repeat1] = aux_sym_words_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [aux_sym_quote_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_heading1_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_heading2_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_heading3_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_heading4_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_unordered_list_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_marker_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_todo_item_done_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_todo_item_pending_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_todo_item_undone_token1] = {
    .visible = false,
    .named = false,
  },
  [sym__eof] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_LF] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_STAR] = {
    .visible = true,
    .named = false,
  },
  [anon_sym__] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BQUOTE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_PIPE] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_CARET] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_BSLASH] = {
    .visible = true,
    .named = false,
  },
  [aux_sym_words_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_words_token2] = {
    .visible = false,
    .named = false,
  },
  [sym_trailing_modifier] = {
    .visible = true,
    .named = true,
  },
  [sym_document] = {
    .visible = true,
    .named = true,
  },
  [sym_quote] = {
    .visible = true,
    .named = true,
  },
  [sym_heading1] = {
    .visible = true,
    .named = true,
  },
  [sym_heading2] = {
    .visible = true,
    .named = true,
  },
  [sym_heading3] = {
    .visible = true,
    .named = true,
  },
  [sym_heading4] = {
    .visible = true,
    .named = true,
  },
  [sym_unordered_list] = {
    .visible = true,
    .named = true,
  },
  [sym_marker] = {
    .visible = true,
    .named = true,
  },
  [sym_todo_item_done] = {
    .visible = true,
    .named = true,
  },
  [sym_todo_item_pending] = {
    .visible = true,
    .named = true,
  },
  [sym_todo_item_undone] = {
    .visible = true,
    .named = true,
  },
  [sym_paragraph] = {
    .visible = true,
    .named = true,
  },
  [sym__soft_paragraph_break] = {
    .visible = false,
    .named = true,
  },
  [sym_words] = {
    .visible = true,
    .named = true,
  },
  [sym_paragraph_segment] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_document_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_paragraph_repeat1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_paragraph_repeat2] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_words_repeat1] = {
    .visible = false,
    .named = false,
  },
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(19);
      if (lookahead == 0) ADVANCE(30);
      if (lookahead == '*') ADVANCE(32);
      if (lookahead == '-') ADVANCE(40);
      if (lookahead == '>') ADVANCE(41);
      if (lookahead == '\\') ADVANCE(37);
      if (lookahead == '^') ADVANCE(36);
      if (lookahead == '_') ADVANCE(33);
      if (lookahead == '`') ADVANCE(34);
      if (lookahead == '|') ADVANCE(35);
      if (lookahead == '~') ADVANCE(45);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(17)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 1:
      if (lookahead == 0) ADVANCE(30);
      if (lookahead == '\n') ADVANCE(31);
      if (lookahead == '~') ADVANCE(45);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(44);
      if (lookahead != 0) ADVANCE(43);
      END_STATE();
    case 2:
      if (lookahead == '\n') SKIP(2)
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(44);
      if (lookahead != 0) ADVANCE(43);
      END_STATE();
    case 3:
      if (lookahead == '*') ADVANCE(10);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(23);
      END_STATE();
    case 4:
      if (lookahead == '*') ADVANCE(8);
      if (lookahead == ']') ADVANCE(13);
      if (lookahead == 'x') ADVANCE(9);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(4);
      END_STATE();
    case 5:
      if (lookahead == '*') ADVANCE(8);
      if (lookahead == 'x') ADVANCE(9);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(4);
      END_STATE();
    case 6:
      if (lookahead == '*') ADVANCE(39);
      if (lookahead == '-') ADVANCE(40);
      if (lookahead == '>') ADVANCE(41);
      if (lookahead == '|') ADVANCE(42);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(6)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 7:
      if (lookahead == '*') ADVANCE(3);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(22);
      END_STATE();
    case 8:
      if (lookahead == ']') ADVANCE(11);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(8);
      END_STATE();
    case 9:
      if (lookahead == ']') ADVANCE(12);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(9);
      END_STATE();
    case 10:
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(24);
      END_STATE();
    case 11:
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(28);
      END_STATE();
    case 12:
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(27);
      END_STATE();
    case 13:
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(29);
      END_STATE();
    case 14:
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(14)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 15:
      if (eof) ADVANCE(19);
      if (lookahead == 0) ADVANCE(30);
      if (lookahead == '\n') ADVANCE(31);
      if (lookahead == '*') ADVANCE(39);
      if (lookahead == '-') ADVANCE(40);
      if (lookahead == '>') ADVANCE(41);
      if (lookahead == '|') ADVANCE(42);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(18)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 16:
      if (eof) ADVANCE(19);
      if (lookahead == '\n') ADVANCE(31);
      if (lookahead == '*') ADVANCE(39);
      if (lookahead == '-') ADVANCE(40);
      if (lookahead == '>') ADVANCE(41);
      if (lookahead == '|') ADVANCE(42);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(18)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 17:
      if (eof) ADVANCE(19);
      if (lookahead == '*') ADVANCE(32);
      if (lookahead == '-') ADVANCE(40);
      if (lookahead == '>') ADVANCE(41);
      if (lookahead == '^') ADVANCE(36);
      if (lookahead == '_') ADVANCE(33);
      if (lookahead == '`') ADVANCE(34);
      if (lookahead == '|') ADVANCE(35);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(17)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 18:
      if (eof) ADVANCE(19);
      if (lookahead == '*') ADVANCE(39);
      if (lookahead == '-') ADVANCE(40);
      if (lookahead == '>') ADVANCE(41);
      if (lookahead == '|') ADVANCE(42);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(18)
      if (lookahead != 0) ADVANCE(38);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(aux_sym_quote_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(20);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(aux_sym_heading1_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(21);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(aux_sym_heading2_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(22);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(aux_sym_heading3_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(23);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(aux_sym_heading4_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(24);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(aux_sym_unordered_list_token1);
      if (lookahead == '[') ADVANCE(5);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(25);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(aux_sym_marker_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(26);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(aux_sym_todo_item_done_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(27);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(aux_sym_todo_item_pending_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(28);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(aux_sym_todo_item_undone_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(29);
      END_STATE();
    case 30:
      ACCEPT_TOKEN(sym__eof);
      END_STATE();
    case 31:
      ACCEPT_TOKEN(anon_sym_LF);
      END_STATE();
    case 32:
      ACCEPT_TOKEN(anon_sym_STAR);
      if (lookahead == '*') ADVANCE(7);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(21);
      END_STATE();
    case 33:
      ACCEPT_TOKEN(anon_sym__);
      END_STATE();
    case 34:
      ACCEPT_TOKEN(anon_sym_BQUOTE);
      END_STATE();
    case 35:
      ACCEPT_TOKEN(anon_sym_PIPE);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(26);
      END_STATE();
    case 36:
      ACCEPT_TOKEN(anon_sym_CARET);
      END_STATE();
    case 37:
      ACCEPT_TOKEN(anon_sym_BSLASH);
      END_STATE();
    case 38:
      ACCEPT_TOKEN(aux_sym_words_token1);
      END_STATE();
    case 39:
      ACCEPT_TOKEN(aux_sym_words_token1);
      if (lookahead == '*') ADVANCE(7);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(21);
      END_STATE();
    case 40:
      ACCEPT_TOKEN(aux_sym_words_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(25);
      END_STATE();
    case 41:
      ACCEPT_TOKEN(aux_sym_words_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(20);
      END_STATE();
    case 42:
      ACCEPT_TOKEN(aux_sym_words_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(26);
      END_STATE();
    case 43:
      ACCEPT_TOKEN(aux_sym_words_token2);
      END_STATE();
    case 44:
      ACCEPT_TOKEN(aux_sym_words_token2);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(44);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(43);
      END_STATE();
    case 45:
      ACCEPT_TOKEN(sym_trailing_modifier);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 16},
  [2] = {.lex_state = 16},
  [3] = {.lex_state = 16},
  [4] = {.lex_state = 15},
  [5] = {.lex_state = 15},
  [6] = {.lex_state = 6},
  [7] = {.lex_state = 6},
  [8] = {.lex_state = 15},
  [9] = {.lex_state = 15},
  [10] = {.lex_state = 15},
  [11] = {.lex_state = 15},
  [12] = {.lex_state = 15},
  [13] = {.lex_state = 15},
  [14] = {.lex_state = 15},
  [15] = {.lex_state = 15},
  [16] = {.lex_state = 15},
  [17] = {.lex_state = 15},
  [18] = {.lex_state = 15},
  [19] = {.lex_state = 15},
  [20] = {.lex_state = 15},
  [21] = {.lex_state = 16},
  [22] = {.lex_state = 15},
  [23] = {.lex_state = 15},
  [24] = {.lex_state = 15},
  [25] = {.lex_state = 15},
  [26] = {.lex_state = 15},
  [27] = {.lex_state = 6},
  [28] = {.lex_state = 6},
  [29] = {.lex_state = 6},
  [30] = {.lex_state = 6},
  [31] = {.lex_state = 6},
  [32] = {.lex_state = 6},
  [33] = {.lex_state = 1},
  [34] = {.lex_state = 1},
  [35] = {.lex_state = 1},
  [36] = {.lex_state = 1},
  [37] = {.lex_state = 1},
  [38] = {.lex_state = 1},
  [39] = {.lex_state = 1},
  [40] = {.lex_state = 14},
  [41] = {.lex_state = 14},
  [42] = {.lex_state = 14},
  [43] = {.lex_state = 14},
  [44] = {.lex_state = 14},
  [45] = {.lex_state = 14},
  [46] = {.lex_state = 14},
  [47] = {.lex_state = 14},
  [48] = {.lex_state = 14},
  [49] = {.lex_state = 14},
  [50] = {.lex_state = 14},
  [51] = {.lex_state = 14},
  [52] = {.lex_state = 14},
  [53] = {.lex_state = 14},
  [54] = {.lex_state = 14},
  [55] = {.lex_state = 2},
  [56] = {.lex_state = 0},
  [57] = {.lex_state = 16},
  [58] = {.lex_state = 16},
  [59] = {.lex_state = 16},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [aux_sym_quote_token1] = ACTIONS(1),
    [aux_sym_heading1_token1] = ACTIONS(1),
    [aux_sym_heading2_token1] = ACTIONS(1),
    [aux_sym_heading3_token1] = ACTIONS(1),
    [aux_sym_heading4_token1] = ACTIONS(1),
    [aux_sym_unordered_list_token1] = ACTIONS(1),
    [aux_sym_marker_token1] = ACTIONS(1),
    [aux_sym_todo_item_done_token1] = ACTIONS(1),
    [aux_sym_todo_item_pending_token1] = ACTIONS(1),
    [aux_sym_todo_item_undone_token1] = ACTIONS(1),
    [sym__eof] = ACTIONS(1),
    [anon_sym_STAR] = ACTIONS(1),
    [anon_sym__] = ACTIONS(1),
    [anon_sym_BQUOTE] = ACTIONS(1),
    [anon_sym_PIPE] = ACTIONS(1),
    [anon_sym_CARET] = ACTIONS(1),
    [anon_sym_BSLASH] = ACTIONS(1),
    [aux_sym_words_token1] = ACTIONS(1),
    [sym_trailing_modifier] = ACTIONS(1),
  },
  [1] = {
    [sym_document] = STATE(56),
    [sym_quote] = STATE(12),
    [sym_heading1] = STATE(6),
    [sym_heading2] = STATE(6),
    [sym_heading3] = STATE(6),
    [sym_heading4] = STATE(6),
    [sym_unordered_list] = STATE(4),
    [sym_marker] = STATE(12),
    [sym_todo_item_done] = STATE(4),
    [sym_todo_item_pending] = STATE(4),
    [sym_todo_item_undone] = STATE(4),
    [sym_paragraph] = STATE(3),
    [sym__soft_paragraph_break] = STATE(3),
    [sym_words] = STATE(35),
    [sym_paragraph_segment] = STATE(4),
    [aux_sym_document_repeat1] = STATE(3),
    [aux_sym_paragraph_repeat1] = STATE(6),
    [aux_sym_paragraph_repeat2] = STATE(4),
    [ts_builtin_sym_end] = ACTIONS(3),
    [aux_sym_quote_token1] = ACTIONS(5),
    [aux_sym_heading1_token1] = ACTIONS(7),
    [aux_sym_heading2_token1] = ACTIONS(9),
    [aux_sym_heading3_token1] = ACTIONS(11),
    [aux_sym_heading4_token1] = ACTIONS(13),
    [aux_sym_unordered_list_token1] = ACTIONS(15),
    [aux_sym_marker_token1] = ACTIONS(17),
    [aux_sym_todo_item_done_token1] = ACTIONS(19),
    [aux_sym_todo_item_pending_token1] = ACTIONS(21),
    [aux_sym_todo_item_undone_token1] = ACTIONS(23),
    [anon_sym_LF] = ACTIONS(25),
    [aux_sym_words_token1] = ACTIONS(27),
  },
  [2] = {
    [sym_quote] = STATE(12),
    [sym_heading1] = STATE(6),
    [sym_heading2] = STATE(6),
    [sym_heading3] = STATE(6),
    [sym_heading4] = STATE(6),
    [sym_unordered_list] = STATE(4),
    [sym_marker] = STATE(12),
    [sym_todo_item_done] = STATE(4),
    [sym_todo_item_pending] = STATE(4),
    [sym_todo_item_undone] = STATE(4),
    [sym_paragraph] = STATE(2),
    [sym__soft_paragraph_break] = STATE(2),
    [sym_words] = STATE(35),
    [sym_paragraph_segment] = STATE(4),
    [aux_sym_document_repeat1] = STATE(2),
    [aux_sym_paragraph_repeat1] = STATE(6),
    [aux_sym_paragraph_repeat2] = STATE(4),
    [ts_builtin_sym_end] = ACTIONS(29),
    [aux_sym_quote_token1] = ACTIONS(31),
    [aux_sym_heading1_token1] = ACTIONS(34),
    [aux_sym_heading2_token1] = ACTIONS(37),
    [aux_sym_heading3_token1] = ACTIONS(40),
    [aux_sym_heading4_token1] = ACTIONS(43),
    [aux_sym_unordered_list_token1] = ACTIONS(46),
    [aux_sym_marker_token1] = ACTIONS(49),
    [aux_sym_todo_item_done_token1] = ACTIONS(52),
    [aux_sym_todo_item_pending_token1] = ACTIONS(55),
    [aux_sym_todo_item_undone_token1] = ACTIONS(58),
    [anon_sym_LF] = ACTIONS(61),
    [aux_sym_words_token1] = ACTIONS(64),
  },
  [3] = {
    [sym_quote] = STATE(12),
    [sym_heading1] = STATE(6),
    [sym_heading2] = STATE(6),
    [sym_heading3] = STATE(6),
    [sym_heading4] = STATE(6),
    [sym_unordered_list] = STATE(4),
    [sym_marker] = STATE(12),
    [sym_todo_item_done] = STATE(4),
    [sym_todo_item_pending] = STATE(4),
    [sym_todo_item_undone] = STATE(4),
    [sym_paragraph] = STATE(2),
    [sym__soft_paragraph_break] = STATE(2),
    [sym_words] = STATE(35),
    [sym_paragraph_segment] = STATE(4),
    [aux_sym_document_repeat1] = STATE(2),
    [aux_sym_paragraph_repeat1] = STATE(6),
    [aux_sym_paragraph_repeat2] = STATE(4),
    [ts_builtin_sym_end] = ACTIONS(67),
    [aux_sym_quote_token1] = ACTIONS(5),
    [aux_sym_heading1_token1] = ACTIONS(7),
    [aux_sym_heading2_token1] = ACTIONS(9),
    [aux_sym_heading3_token1] = ACTIONS(11),
    [aux_sym_heading4_token1] = ACTIONS(13),
    [aux_sym_unordered_list_token1] = ACTIONS(15),
    [aux_sym_marker_token1] = ACTIONS(17),
    [aux_sym_todo_item_done_token1] = ACTIONS(19),
    [aux_sym_todo_item_pending_token1] = ACTIONS(21),
    [aux_sym_todo_item_undone_token1] = ACTIONS(23),
    [anon_sym_LF] = ACTIONS(69),
    [aux_sym_words_token1] = ACTIONS(27),
  },
  [4] = {
    [sym_quote] = STATE(5),
    [sym_heading1] = STATE(6),
    [sym_heading2] = STATE(6),
    [sym_heading3] = STATE(6),
    [sym_heading4] = STATE(6),
    [sym_unordered_list] = STATE(5),
    [sym_marker] = STATE(5),
    [sym_todo_item_done] = STATE(5),
    [sym_todo_item_pending] = STATE(5),
    [sym_todo_item_undone] = STATE(5),
    [sym__soft_paragraph_break] = STATE(21),
    [sym_words] = STATE(35),
    [sym_paragraph_segment] = STATE(5),
    [aux_sym_paragraph_repeat1] = STATE(6),
    [aux_sym_paragraph_repeat2] = STATE(5),
    [aux_sym_quote_token1] = ACTIONS(71),
    [aux_sym_heading1_token1] = ACTIONS(7),
    [aux_sym_heading2_token1] = ACTIONS(9),
    [aux_sym_heading3_token1] = ACTIONS(11),
    [aux_sym_heading4_token1] = ACTIONS(13),
    [aux_sym_unordered_list_token1] = ACTIONS(15),
    [aux_sym_marker_token1] = ACTIONS(73),
    [aux_sym_todo_item_done_token1] = ACTIONS(19),
    [aux_sym_todo_item_pending_token1] = ACTIONS(21),
    [aux_sym_todo_item_undone_token1] = ACTIONS(23),
    [sym__eof] = ACTIONS(75),
    [anon_sym_LF] = ACTIONS(75),
    [aux_sym_words_token1] = ACTIONS(27),
  },
  [5] = {
    [sym_quote] = STATE(5),
    [sym_heading1] = STATE(6),
    [sym_heading2] = STATE(6),
    [sym_heading3] = STATE(6),
    [sym_heading4] = STATE(6),
    [sym_unordered_list] = STATE(5),
    [sym_marker] = STATE(5),
    [sym_todo_item_done] = STATE(5),
    [sym_todo_item_pending] = STATE(5),
    [sym_todo_item_undone] = STATE(5),
    [sym_words] = STATE(35),
    [sym_paragraph_segment] = STATE(5),
    [aux_sym_paragraph_repeat1] = STATE(6),
    [aux_sym_paragraph_repeat2] = STATE(5),
    [aux_sym_quote_token1] = ACTIONS(77),
    [aux_sym_heading1_token1] = ACTIONS(80),
    [aux_sym_heading2_token1] = ACTIONS(83),
    [aux_sym_heading3_token1] = ACTIONS(86),
    [aux_sym_heading4_token1] = ACTIONS(89),
    [aux_sym_unordered_list_token1] = ACTIONS(92),
    [aux_sym_marker_token1] = ACTIONS(95),
    [aux_sym_todo_item_done_token1] = ACTIONS(98),
    [aux_sym_todo_item_pending_token1] = ACTIONS(101),
    [aux_sym_todo_item_undone_token1] = ACTIONS(104),
    [sym__eof] = ACTIONS(107),
    [anon_sym_LF] = ACTIONS(107),
    [aux_sym_words_token1] = ACTIONS(109),
  },
  [6] = {
    [sym_quote] = STATE(22),
    [sym_heading1] = STATE(7),
    [sym_heading2] = STATE(7),
    [sym_heading3] = STATE(7),
    [sym_heading4] = STATE(7),
    [sym_unordered_list] = STATE(22),
    [sym_marker] = STATE(22),
    [sym_todo_item_done] = STATE(22),
    [sym_todo_item_pending] = STATE(22),
    [sym_todo_item_undone] = STATE(22),
    [sym_words] = STATE(35),
    [sym_paragraph_segment] = STATE(22),
    [aux_sym_paragraph_repeat1] = STATE(7),
    [aux_sym_quote_token1] = ACTIONS(112),
    [aux_sym_heading1_token1] = ACTIONS(114),
    [aux_sym_heading2_token1] = ACTIONS(116),
    [aux_sym_heading3_token1] = ACTIONS(118),
    [aux_sym_heading4_token1] = ACTIONS(120),
    [aux_sym_unordered_list_token1] = ACTIONS(15),
    [aux_sym_marker_token1] = ACTIONS(122),
    [aux_sym_todo_item_done_token1] = ACTIONS(124),
    [aux_sym_todo_item_pending_token1] = ACTIONS(126),
    [aux_sym_todo_item_undone_token1] = ACTIONS(128),
    [aux_sym_words_token1] = ACTIONS(27),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 7,
    ACTIONS(132), 1,
      aux_sym_heading1_token1,
    ACTIONS(135), 1,
      aux_sym_heading2_token1,
    ACTIONS(138), 1,
      aux_sym_heading3_token1,
    ACTIONS(141), 1,
      aux_sym_heading4_token1,
    ACTIONS(144), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(130), 5,
      aux_sym_quote_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
    STATE(7), 5,
      sym_heading1,
      sym_heading2,
      sym_heading3,
      sym_heading4,
      aux_sym_paragraph_repeat1,
  [31] = 4,
    ACTIONS(5), 1,
      aux_sym_quote_token1,
    STATE(13), 1,
      sym_quote,
    ACTIONS(146), 3,
      ts_builtin_sym_end,
      sym__eof,
      anon_sym_LF,
    ACTIONS(148), 10,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [55] = 4,
    ACTIONS(71), 1,
      aux_sym_quote_token1,
    STATE(17), 1,
      sym_quote,
    ACTIONS(146), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(148), 10,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [78] = 2,
    ACTIONS(150), 3,
      ts_builtin_sym_end,
      sym__eof,
      anon_sym_LF,
    ACTIONS(152), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [97] = 2,
    ACTIONS(146), 3,
      ts_builtin_sym_end,
      sym__eof,
      anon_sym_LF,
    ACTIONS(148), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [116] = 3,
    ACTIONS(158), 1,
      sym__eof,
    ACTIONS(154), 2,
      ts_builtin_sym_end,
      anon_sym_LF,
    ACTIONS(156), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [137] = 2,
    ACTIONS(160), 3,
      ts_builtin_sym_end,
      sym__eof,
      anon_sym_LF,
    ACTIONS(162), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [156] = 2,
    ACTIONS(164), 3,
      ts_builtin_sym_end,
      sym__eof,
      anon_sym_LF,
    ACTIONS(166), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [175] = 2,
    ACTIONS(168), 3,
      ts_builtin_sym_end,
      sym__eof,
      anon_sym_LF,
    ACTIONS(170), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [194] = 2,
    ACTIONS(174), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(172), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [212] = 2,
    ACTIONS(160), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(162), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [230] = 2,
    ACTIONS(168), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(170), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [248] = 2,
    ACTIONS(164), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(166), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [266] = 2,
    ACTIONS(150), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(152), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [284] = 2,
    ACTIONS(176), 2,
      ts_builtin_sym_end,
      anon_sym_LF,
    ACTIONS(178), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [302] = 2,
    ACTIONS(107), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(180), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [320] = 2,
    ACTIONS(146), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(148), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [338] = 2,
    ACTIONS(184), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(182), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [356] = 2,
    ACTIONS(188), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(186), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [374] = 2,
    ACTIONS(192), 2,
      sym__eof,
      anon_sym_LF,
    ACTIONS(190), 11,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_unordered_list_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
      aux_sym_words_token1,
  [392] = 2,
    ACTIONS(196), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(194), 9,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
  [408] = 2,
    ACTIONS(200), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(198), 9,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
  [424] = 2,
    ACTIONS(204), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(202), 9,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
  [440] = 2,
    ACTIONS(152), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(150), 9,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
  [456] = 2,
    ACTIONS(208), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(206), 9,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
  [472] = 2,
    ACTIONS(148), 2,
      aux_sym_unordered_list_token1,
      aux_sym_words_token1,
    ACTIONS(146), 9,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_heading2_token1,
      aux_sym_heading3_token1,
      aux_sym_heading4_token1,
      aux_sym_marker_token1,
      aux_sym_todo_item_done_token1,
      aux_sym_todo_item_pending_token1,
      aux_sym_todo_item_undone_token1,
  [488] = 3,
    ACTIONS(212), 1,
      aux_sym_words_token2,
    STATE(34), 1,
      aux_sym_words_repeat1,
    ACTIONS(210), 3,
      sym__eof,
      anon_sym_LF,
      sym_trailing_modifier,
  [500] = 3,
    ACTIONS(216), 1,
      aux_sym_words_token2,
    STATE(34), 1,
      aux_sym_words_repeat1,
    ACTIONS(214), 3,
      sym__eof,
      anon_sym_LF,
      sym_trailing_modifier,
  [512] = 3,
    ACTIONS(221), 1,
      sym_trailing_modifier,
    STATE(23), 1,
      sym__soft_paragraph_break,
    ACTIONS(219), 2,
      sym__eof,
      anon_sym_LF,
  [523] = 3,
    ACTIONS(225), 1,
      sym_trailing_modifier,
    STATE(11), 1,
      sym__soft_paragraph_break,
    ACTIONS(223), 2,
      sym__eof,
      anon_sym_LF,
  [534] = 4,
    ACTIONS(219), 1,
      sym__eof,
    ACTIONS(221), 1,
      sym_trailing_modifier,
    ACTIONS(227), 1,
      anon_sym_LF,
    STATE(9), 1,
      sym__soft_paragraph_break,
  [547] = 3,
    ACTIONS(231), 1,
      sym_trailing_modifier,
    STATE(32), 1,
      sym__soft_paragraph_break,
    ACTIONS(229), 2,
      sym__eof,
      anon_sym_LF,
  [558] = 4,
    ACTIONS(223), 1,
      sym__eof,
    ACTIONS(225), 1,
      sym_trailing_modifier,
    ACTIONS(233), 1,
      anon_sym_LF,
    STATE(8), 1,
      sym__soft_paragraph_break,
  [571] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(18), 1,
      sym_paragraph_segment,
    STATE(35), 1,
      sym_words,
  [581] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(27), 1,
      sym_paragraph_segment,
    STATE(38), 1,
      sym_words,
  [591] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(20), 1,
      sym_paragraph_segment,
    STATE(35), 1,
      sym_words,
  [601] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(25), 1,
      sym_paragraph_segment,
    STATE(35), 1,
      sym_words,
  [611] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(31), 1,
      sym_paragraph_segment,
    STATE(38), 1,
      sym_words,
  [621] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(24), 1,
      sym_paragraph_segment,
    STATE(35), 1,
      sym_words,
  [631] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(16), 1,
      sym_paragraph_segment,
    STATE(35), 1,
      sym_words,
  [641] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(15), 1,
      sym_paragraph_segment,
    STATE(36), 1,
      sym_words,
  [651] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(29), 1,
      sym_paragraph_segment,
    STATE(38), 1,
      sym_words,
  [661] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(19), 1,
      sym_paragraph_segment,
    STATE(37), 1,
      sym_words,
  [671] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(28), 1,
      sym_paragraph_segment,
    STATE(38), 1,
      sym_words,
  [681] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(26), 1,
      sym_paragraph_segment,
    STATE(35), 1,
      sym_words,
  [691] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(14), 1,
      sym_paragraph_segment,
    STATE(39), 1,
      sym_words,
  [701] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(10), 1,
      sym_paragraph_segment,
    STATE(36), 1,
      sym_words,
  [711] = 3,
    ACTIONS(235), 1,
      aux_sym_words_token1,
    STATE(30), 1,
      sym_paragraph_segment,
    STATE(38), 1,
      sym_words,
  [721] = 2,
    ACTIONS(237), 1,
      aux_sym_words_token2,
    STATE(33), 1,
      aux_sym_words_repeat1,
  [728] = 1,
    ACTIONS(239), 1,
      ts_builtin_sym_end,
  [732] = 1,
    ACTIONS(241), 1,
      anon_sym_LF,
  [736] = 1,
    ACTIONS(243), 1,
      anon_sym_LF,
  [740] = 1,
    ACTIONS(245), 1,
      anon_sym_LF,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(7)] = 0,
  [SMALL_STATE(8)] = 31,
  [SMALL_STATE(9)] = 55,
  [SMALL_STATE(10)] = 78,
  [SMALL_STATE(11)] = 97,
  [SMALL_STATE(12)] = 116,
  [SMALL_STATE(13)] = 137,
  [SMALL_STATE(14)] = 156,
  [SMALL_STATE(15)] = 175,
  [SMALL_STATE(16)] = 194,
  [SMALL_STATE(17)] = 212,
  [SMALL_STATE(18)] = 230,
  [SMALL_STATE(19)] = 248,
  [SMALL_STATE(20)] = 266,
  [SMALL_STATE(21)] = 284,
  [SMALL_STATE(22)] = 302,
  [SMALL_STATE(23)] = 320,
  [SMALL_STATE(24)] = 338,
  [SMALL_STATE(25)] = 356,
  [SMALL_STATE(26)] = 374,
  [SMALL_STATE(27)] = 392,
  [SMALL_STATE(28)] = 408,
  [SMALL_STATE(29)] = 424,
  [SMALL_STATE(30)] = 440,
  [SMALL_STATE(31)] = 456,
  [SMALL_STATE(32)] = 472,
  [SMALL_STATE(33)] = 488,
  [SMALL_STATE(34)] = 500,
  [SMALL_STATE(35)] = 512,
  [SMALL_STATE(36)] = 523,
  [SMALL_STATE(37)] = 534,
  [SMALL_STATE(38)] = 547,
  [SMALL_STATE(39)] = 558,
  [SMALL_STATE(40)] = 571,
  [SMALL_STATE(41)] = 581,
  [SMALL_STATE(42)] = 591,
  [SMALL_STATE(43)] = 601,
  [SMALL_STATE(44)] = 611,
  [SMALL_STATE(45)] = 621,
  [SMALL_STATE(46)] = 631,
  [SMALL_STATE(47)] = 641,
  [SMALL_STATE(48)] = 651,
  [SMALL_STATE(49)] = 661,
  [SMALL_STATE(50)] = 671,
  [SMALL_STATE(51)] = 681,
  [SMALL_STATE(52)] = 691,
  [SMALL_STATE(53)] = 701,
  [SMALL_STATE(54)] = 711,
  [SMALL_STATE(55)] = 721,
  [SMALL_STATE(56)] = 728,
  [SMALL_STATE(57)] = 732,
  [SMALL_STATE(58)] = 736,
  [SMALL_STATE(59)] = 740,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_document, 0),
  [5] = {.entry = {.count = 1, .reusable = false}}, SHIFT(52),
  [7] = {.entry = {.count = 1, .reusable = false}}, SHIFT(44),
  [9] = {.entry = {.count = 1, .reusable = false}}, SHIFT(41),
  [11] = {.entry = {.count = 1, .reusable = false}}, SHIFT(48),
  [13] = {.entry = {.count = 1, .reusable = false}}, SHIFT(50),
  [15] = {.entry = {.count = 1, .reusable = false}}, SHIFT(51),
  [17] = {.entry = {.count = 1, .reusable = false}}, SHIFT(47),
  [19] = {.entry = {.count = 1, .reusable = false}}, SHIFT(46),
  [21] = {.entry = {.count = 1, .reusable = false}}, SHIFT(45),
  [23] = {.entry = {.count = 1, .reusable = false}}, SHIFT(43),
  [25] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [27] = {.entry = {.count = 1, .reusable = false}}, SHIFT(55),
  [29] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 2),
  [31] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(52),
  [34] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(44),
  [37] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(41),
  [40] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(48),
  [43] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(50),
  [46] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(51),
  [49] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(47),
  [52] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(46),
  [55] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(45),
  [58] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(43),
  [61] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(2),
  [64] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(55),
  [67] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_document, 1),
  [69] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [71] = {.entry = {.count = 1, .reusable = false}}, SHIFT(49),
  [73] = {.entry = {.count = 1, .reusable = false}}, SHIFT(40),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(21),
  [77] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(49),
  [80] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(44),
  [83] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(41),
  [86] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(48),
  [89] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(50),
  [92] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(51),
  [95] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(40),
  [98] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(46),
  [101] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(45),
  [104] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(43),
  [107] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat2, 2),
  [109] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2), SHIFT_REPEAT(55),
  [112] = {.entry = {.count = 1, .reusable = true}}, SHIFT(49),
  [114] = {.entry = {.count = 1, .reusable = true}}, SHIFT(44),
  [116] = {.entry = {.count = 1, .reusable = true}}, SHIFT(41),
  [118] = {.entry = {.count = 1, .reusable = true}}, SHIFT(48),
  [120] = {.entry = {.count = 1, .reusable = true}}, SHIFT(50),
  [122] = {.entry = {.count = 1, .reusable = true}}, SHIFT(40),
  [124] = {.entry = {.count = 1, .reusable = true}}, SHIFT(46),
  [126] = {.entry = {.count = 1, .reusable = true}}, SHIFT(45),
  [128] = {.entry = {.count = 1, .reusable = true}}, SHIFT(43),
  [130] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat1, 2),
  [132] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat1, 2), SHIFT_REPEAT(44),
  [135] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat1, 2), SHIFT_REPEAT(41),
  [138] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat1, 2), SHIFT_REPEAT(48),
  [141] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat1, 2), SHIFT_REPEAT(50),
  [144] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat1, 2),
  [146] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paragraph_segment, 2),
  [148] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_paragraph_segment, 2),
  [150] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paragraph_segment, 4),
  [152] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_paragraph_segment, 4),
  [154] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 1),
  [156] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 1),
  [158] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_paragraph_repeat2, 1),
  [160] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_quote, 4),
  [162] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_quote, 4),
  [164] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_quote, 2),
  [166] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_quote, 2),
  [168] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_marker, 2),
  [170] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_marker, 2),
  [172] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_todo_item_done, 2),
  [174] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_todo_item_done, 2),
  [176] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paragraph, 2),
  [178] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_paragraph, 2),
  [180] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_paragraph_repeat2, 2),
  [182] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_todo_item_pending, 2),
  [184] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_todo_item_pending, 2),
  [186] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_todo_item_undone, 2),
  [188] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_todo_item_undone, 2),
  [190] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_unordered_list, 2),
  [192] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_unordered_list, 2),
  [194] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_heading2, 2),
  [196] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_heading2, 2),
  [198] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_heading4, 2),
  [200] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_heading4, 2),
  [202] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_heading3, 2),
  [204] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_heading3, 2),
  [206] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_heading1, 2),
  [208] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_heading1, 2),
  [210] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_words, 2),
  [212] = {.entry = {.count = 1, .reusable = false}}, SHIFT(34),
  [214] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_words_repeat1, 2),
  [216] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_words_repeat1, 2), SHIFT_REPEAT(34),
  [219] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [221] = {.entry = {.count = 1, .reusable = true}}, SHIFT(57),
  [223] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [225] = {.entry = {.count = 1, .reusable = true}}, SHIFT(58),
  [227] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [229] = {.entry = {.count = 1, .reusable = true}}, SHIFT(32),
  [231] = {.entry = {.count = 1, .reusable = true}}, SHIFT(59),
  [233] = {.entry = {.count = 1, .reusable = true}}, SHIFT(8),
  [235] = {.entry = {.count = 1, .reusable = true}}, SHIFT(55),
  [237] = {.entry = {.count = 1, .reusable = true}}, SHIFT(33),
  [239] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [241] = {.entry = {.count = 1, .reusable = true}}, SHIFT(42),
  [243] = {.entry = {.count = 1, .reusable = true}}, SHIFT(53),
  [245] = {.entry = {.count = 1, .reusable = true}}, SHIFT(54),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_norg(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
