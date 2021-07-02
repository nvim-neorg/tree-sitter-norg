#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 13
#define STATE_COUNT 28
#define LARGE_STATE_COUNT 4
#define SYMBOL_COUNT 18
#define ALIAS_COUNT 0
#define TOKEN_COUNT 10
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 4
#define PRODUCTION_ID_COUNT 1

enum {
  aux_sym_quote_token1 = 1,
  aux_sym_heading1_token1 = 2,
  anon_sym_LF_LF = 3,
  aux_sym_words_token1 = 4,
  aux_sym_words_token2 = 5,
  sym__eof = 6,
  sym__soft_paragraph_break = 7,
  anon_sym_LF = 8,
  sym_trailing_modifier = 9,
  sym_document = 10,
  sym_quote = 11,
  sym_heading1 = 12,
  sym_words = 13,
  sym_paragraph_segment = 14,
  aux_sym_document_repeat1 = 15,
  aux_sym_heading1_repeat1 = 16,
  aux_sym_words_repeat1 = 17,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [aux_sym_quote_token1] = "quote_token1",
  [aux_sym_heading1_token1] = "heading1_token1",
  [anon_sym_LF_LF] = "\n\n",
  [aux_sym_words_token1] = "words_token1",
  [aux_sym_words_token2] = "words_token2",
  [sym__eof] = "_eof",
  [sym__soft_paragraph_break] = "_soft_paragraph_break",
  [anon_sym_LF] = "\n",
  [sym_trailing_modifier] = "trailing_modifier",
  [sym_document] = "document",
  [sym_quote] = "quote",
  [sym_heading1] = "heading1",
  [sym_words] = "words",
  [sym_paragraph_segment] = "paragraph_segment",
  [aux_sym_document_repeat1] = "document_repeat1",
  [aux_sym_heading1_repeat1] = "heading1_repeat1",
  [aux_sym_words_repeat1] = "words_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [aux_sym_quote_token1] = aux_sym_quote_token1,
  [aux_sym_heading1_token1] = aux_sym_heading1_token1,
  [anon_sym_LF_LF] = anon_sym_LF_LF,
  [aux_sym_words_token1] = aux_sym_words_token1,
  [aux_sym_words_token2] = aux_sym_words_token2,
  [sym__eof] = sym__eof,
  [sym__soft_paragraph_break] = sym__soft_paragraph_break,
  [anon_sym_LF] = anon_sym_LF,
  [sym_trailing_modifier] = sym_trailing_modifier,
  [sym_document] = sym_document,
  [sym_quote] = sym_quote,
  [sym_heading1] = sym_heading1,
  [sym_words] = sym_words,
  [sym_paragraph_segment] = sym_paragraph_segment,
  [aux_sym_document_repeat1] = aux_sym_document_repeat1,
  [aux_sym_heading1_repeat1] = aux_sym_heading1_repeat1,
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
  [anon_sym_LF_LF] = {
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
  [sym__eof] = {
    .visible = false,
    .named = true,
  },
  [sym__soft_paragraph_break] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_LF] = {
    .visible = true,
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
  [aux_sym_heading1_repeat1] = {
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
      if (eof) ADVANCE(10);
      if (lookahead == 0) ADVANCE(19);
      if (lookahead == '\n') ADVANCE(20);
      if (lookahead == '*') ADVANCE(15);
      if (lookahead == '>') ADVANCE(16);
      if (lookahead == '~') ADVANCE(22);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(9)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 1:
      if (lookahead == 0) ADVANCE(19);
      if (lookahead == '\n') ADVANCE(20);
      if (lookahead == '~') ADVANCE(22);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(18);
      if (lookahead != 0) ADVANCE(17);
      END_STATE();
    case 2:
      if (lookahead == 0) ADVANCE(19);
      if (lookahead == '\n') ADVANCE(4);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(3)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 3:
      if (lookahead == '\n') ADVANCE(4);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(3)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 4:
      if (lookahead == '\n') ADVANCE(13);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(3)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 5:
      if (lookahead == '\n') SKIP(5)
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(18);
      if (lookahead != 0) ADVANCE(17);
      END_STATE();
    case 6:
      if (lookahead == '\n') ADVANCE(21);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(6)
      END_STATE();
    case 7:
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(7)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 8:
      if (eof) ADVANCE(10);
      if (lookahead == '\n') ADVANCE(20);
      if (lookahead == '*') ADVANCE(15);
      if (lookahead == '>') ADVANCE(16);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(9)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 9:
      if (eof) ADVANCE(10);
      if (lookahead == '*') ADVANCE(15);
      if (lookahead == '>') ADVANCE(16);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(9)
      if (lookahead != 0) ADVANCE(14);
      END_STATE();
    case 10:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 11:
      ACCEPT_TOKEN(aux_sym_quote_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(11);
      END_STATE();
    case 12:
      ACCEPT_TOKEN(aux_sym_heading1_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(12);
      END_STATE();
    case 13:
      ACCEPT_TOKEN(anon_sym_LF_LF);
      if (lookahead == '\n') ADVANCE(13);
      END_STATE();
    case 14:
      ACCEPT_TOKEN(aux_sym_words_token1);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(aux_sym_words_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(12);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(aux_sym_words_token1);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(11);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(aux_sym_words_token2);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(aux_sym_words_token2);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(18);
      if (lookahead != 0 &&
          lookahead != '\n') ADVANCE(17);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(sym__eof);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(sym__soft_paragraph_break);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(anon_sym_LF);
      if (lookahead == '\n') ADVANCE(21);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(sym_trailing_modifier);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 8},
  [2] = {.lex_state = 8},
  [3] = {.lex_state = 8},
  [4] = {.lex_state = 8},
  [5] = {.lex_state = 2},
  [6] = {.lex_state = 2},
  [7] = {.lex_state = 8},
  [8] = {.lex_state = 8},
  [9] = {.lex_state = 1},
  [10] = {.lex_state = 8},
  [11] = {.lex_state = 1},
  [12] = {.lex_state = 8},
  [13] = {.lex_state = 8},
  [14] = {.lex_state = 8},
  [15] = {.lex_state = 1},
  [16] = {.lex_state = 7},
  [17] = {.lex_state = 7},
  [18] = {.lex_state = 7},
  [19] = {.lex_state = 1},
  [20] = {.lex_state = 2},
  [21] = {.lex_state = 2},
  [22] = {.lex_state = 1},
  [23] = {.lex_state = 7},
  [24] = {.lex_state = 5},
  [25] = {.lex_state = 6},
  [26] = {.lex_state = 6},
  [27] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [aux_sym_quote_token1] = ACTIONS(1),
    [aux_sym_heading1_token1] = ACTIONS(1),
    [aux_sym_words_token1] = ACTIONS(1),
    [sym__eof] = ACTIONS(1),
    [sym__soft_paragraph_break] = ACTIONS(1),
    [sym_trailing_modifier] = ACTIONS(1),
  },
  [1] = {
    [sym_document] = STATE(27),
    [sym_quote] = STATE(3),
    [sym_heading1] = STATE(3),
    [sym_words] = STATE(19),
    [sym_paragraph_segment] = STATE(3),
    [aux_sym_document_repeat1] = STATE(3),
    [ts_builtin_sym_end] = ACTIONS(3),
    [aux_sym_quote_token1] = ACTIONS(5),
    [aux_sym_heading1_token1] = ACTIONS(7),
    [aux_sym_words_token1] = ACTIONS(9),
    [sym__soft_paragraph_break] = ACTIONS(11),
  },
  [2] = {
    [sym_quote] = STATE(2),
    [sym_heading1] = STATE(2),
    [sym_words] = STATE(19),
    [sym_paragraph_segment] = STATE(2),
    [aux_sym_document_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(13),
    [aux_sym_quote_token1] = ACTIONS(15),
    [aux_sym_heading1_token1] = ACTIONS(18),
    [aux_sym_words_token1] = ACTIONS(21),
    [sym__soft_paragraph_break] = ACTIONS(24),
  },
  [3] = {
    [sym_quote] = STATE(2),
    [sym_heading1] = STATE(2),
    [sym_words] = STATE(19),
    [sym_paragraph_segment] = STATE(2),
    [aux_sym_document_repeat1] = STATE(2),
    [ts_builtin_sym_end] = ACTIONS(27),
    [aux_sym_quote_token1] = ACTIONS(5),
    [aux_sym_heading1_token1] = ACTIONS(7),
    [aux_sym_words_token1] = ACTIONS(9),
    [sym__soft_paragraph_break] = ACTIONS(29),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 4,
    STATE(22), 1,
      sym_words,
    ACTIONS(31), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    STATE(5), 2,
      sym_paragraph_segment,
      aux_sym_heading1_repeat1,
    ACTIONS(33), 3,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [17] = 4,
    ACTIONS(9), 1,
      aux_sym_words_token1,
    STATE(22), 1,
      sym_words,
    ACTIONS(35), 2,
      anon_sym_LF_LF,
      sym__eof,
    STATE(6), 2,
      sym_paragraph_segment,
      aux_sym_heading1_repeat1,
  [32] = 4,
    ACTIONS(39), 1,
      aux_sym_words_token1,
    STATE(22), 1,
      sym_words,
    ACTIONS(37), 2,
      anon_sym_LF_LF,
      sym__eof,
    STATE(6), 2,
      sym_paragraph_segment,
      aux_sym_heading1_repeat1,
  [47] = 4,
    ACTIONS(5), 1,
      aux_sym_quote_token1,
    STATE(12), 1,
      sym_quote,
    ACTIONS(42), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    ACTIONS(44), 2,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [62] = 2,
    ACTIONS(46), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    ACTIONS(48), 3,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [72] = 3,
    ACTIONS(50), 1,
      aux_sym_words_token2,
    STATE(11), 1,
      aux_sym_words_repeat1,
    ACTIONS(52), 3,
      sym__eof,
      sym__soft_paragraph_break,
      sym_trailing_modifier,
  [84] = 2,
    ACTIONS(42), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    ACTIONS(44), 3,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [94] = 3,
    ACTIONS(54), 1,
      aux_sym_words_token2,
    STATE(11), 1,
      aux_sym_words_repeat1,
    ACTIONS(57), 3,
      sym__eof,
      sym__soft_paragraph_break,
      sym_trailing_modifier,
  [106] = 2,
    ACTIONS(59), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    ACTIONS(61), 3,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [116] = 2,
    ACTIONS(63), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    ACTIONS(65), 3,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [126] = 2,
    ACTIONS(67), 2,
      ts_builtin_sym_end,
      sym__soft_paragraph_break,
    ACTIONS(69), 3,
      aux_sym_quote_token1,
      aux_sym_heading1_token1,
      aux_sym_words_token1,
  [136] = 3,
    ACTIONS(71), 1,
      sym__eof,
    ACTIONS(73), 1,
      sym__soft_paragraph_break,
    ACTIONS(75), 1,
      sym_trailing_modifier,
  [146] = 3,
    ACTIONS(77), 1,
      aux_sym_words_token1,
    STATE(4), 1,
      sym_paragraph_segment,
    STATE(19), 1,
      sym_words,
  [156] = 3,
    ACTIONS(77), 1,
      aux_sym_words_token1,
    STATE(14), 1,
      sym_paragraph_segment,
    STATE(19), 1,
      sym_words,
  [166] = 3,
    ACTIONS(77), 1,
      aux_sym_words_token1,
    STATE(8), 1,
      sym_paragraph_segment,
    STATE(15), 1,
      sym_words,
  [176] = 2,
    ACTIONS(75), 1,
      sym_trailing_modifier,
    ACTIONS(71), 2,
      sym__eof,
      sym__soft_paragraph_break,
  [184] = 2,
    ACTIONS(44), 1,
      aux_sym_words_token1,
    ACTIONS(42), 2,
      anon_sym_LF_LF,
      sym__eof,
  [192] = 2,
    ACTIONS(69), 1,
      aux_sym_words_token1,
    ACTIONS(67), 2,
      anon_sym_LF_LF,
      sym__eof,
  [200] = 2,
    ACTIONS(81), 1,
      sym_trailing_modifier,
    ACTIONS(79), 2,
      sym__eof,
      sym__soft_paragraph_break,
  [208] = 3,
    ACTIONS(77), 1,
      aux_sym_words_token1,
    STATE(21), 1,
      sym_paragraph_segment,
    STATE(22), 1,
      sym_words,
  [218] = 2,
    ACTIONS(83), 1,
      aux_sym_words_token2,
    STATE(9), 1,
      aux_sym_words_repeat1,
  [225] = 1,
    ACTIONS(85), 1,
      anon_sym_LF,
  [229] = 1,
    ACTIONS(87), 1,
      anon_sym_LF,
  [233] = 1,
    ACTIONS(89), 1,
      ts_builtin_sym_end,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(4)] = 0,
  [SMALL_STATE(5)] = 17,
  [SMALL_STATE(6)] = 32,
  [SMALL_STATE(7)] = 47,
  [SMALL_STATE(8)] = 62,
  [SMALL_STATE(9)] = 72,
  [SMALL_STATE(10)] = 84,
  [SMALL_STATE(11)] = 94,
  [SMALL_STATE(12)] = 106,
  [SMALL_STATE(13)] = 116,
  [SMALL_STATE(14)] = 126,
  [SMALL_STATE(15)] = 136,
  [SMALL_STATE(16)] = 146,
  [SMALL_STATE(17)] = 156,
  [SMALL_STATE(18)] = 166,
  [SMALL_STATE(19)] = 176,
  [SMALL_STATE(20)] = 184,
  [SMALL_STATE(21)] = 192,
  [SMALL_STATE(22)] = 200,
  [SMALL_STATE(23)] = 208,
  [SMALL_STATE(24)] = 218,
  [SMALL_STATE(25)] = 225,
  [SMALL_STATE(26)] = 229,
  [SMALL_STATE(27)] = 233,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_document, 0),
  [5] = {.entry = {.count = 1, .reusable = false}}, SHIFT(18),
  [7] = {.entry = {.count = 1, .reusable = false}}, SHIFT(16),
  [9] = {.entry = {.count = 1, .reusable = false}}, SHIFT(24),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [13] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 2),
  [15] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(18),
  [18] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(16),
  [21] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(24),
  [24] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(2),
  [27] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_document, 1),
  [29] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [31] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_heading1, 2),
  [33] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_heading1, 2),
  [35] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [37] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_heading1_repeat1, 2),
  [39] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_heading1_repeat1, 2), SHIFT_REPEAT(24),
  [42] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paragraph_segment, 2),
  [44] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_paragraph_segment, 2),
  [46] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_quote, 2),
  [48] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_quote, 2),
  [50] = {.entry = {.count = 1, .reusable = false}}, SHIFT(11),
  [52] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_words, 2),
  [54] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_words_repeat1, 2), SHIFT_REPEAT(11),
  [57] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_words_repeat1, 2),
  [59] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_quote, 4),
  [61] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_quote, 4),
  [63] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_heading1, 4),
  [65] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_heading1, 4),
  [67] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_paragraph_segment, 4),
  [69] = {.entry = {.count = 1, .reusable = false}}, REDUCE(sym_paragraph_segment, 4),
  [71] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [73] = {.entry = {.count = 1, .reusable = true}}, SHIFT(7),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [77] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [79] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [81] = {.entry = {.count = 1, .reusable = true}}, SHIFT(26),
  [83] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [85] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [87] = {.entry = {.count = 1, .reusable = true}}, SHIFT(23),
  [89] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
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
