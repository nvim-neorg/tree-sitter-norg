#include <tree_sitter/parser.h>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 13
#define STATE_COUNT 5
#define LARGE_STATE_COUNT 4
#define SYMBOL_COUNT 13
#define ALIAS_COUNT 0
#define TOKEN_COUNT 11
#define EXTERNAL_TOKEN_COUNT 10
#define FIELD_COUNT 0
#define MAX_ALIAS_SEQUENCE_LENGTH 2
#define PRODUCTION_ID_COUNT 1

enum {
  sym_none = 1,
  sym_paragraph = 2,
  sym_paragraph_segment = 3,
  sym_heading1 = 4,
  sym_heading2 = 5,
  sym_heading3 = 6,
  sym_heading4 = 7,
  sym_heading5 = 8,
  sym_heading6 = 9,
  sym_quote = 10,
  sym_document = 11,
  aux_sym_document_repeat1 = 12,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [sym_none] = "none",
  [sym_paragraph] = "paragraph",
  [sym_paragraph_segment] = "paragraph_segment",
  [sym_heading1] = "heading1",
  [sym_heading2] = "heading2",
  [sym_heading3] = "heading3",
  [sym_heading4] = "heading4",
  [sym_heading5] = "heading5",
  [sym_heading6] = "heading6",
  [sym_quote] = "quote",
  [sym_document] = "document",
  [aux_sym_document_repeat1] = "document_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [sym_none] = sym_none,
  [sym_paragraph] = sym_paragraph,
  [sym_paragraph_segment] = sym_paragraph_segment,
  [sym_heading1] = sym_heading1,
  [sym_heading2] = sym_heading2,
  [sym_heading3] = sym_heading3,
  [sym_heading4] = sym_heading4,
  [sym_heading5] = sym_heading5,
  [sym_heading6] = sym_heading6,
  [sym_quote] = sym_quote,
  [sym_document] = sym_document,
  [aux_sym_document_repeat1] = aux_sym_document_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [sym_none] = {
    .visible = true,
    .named = true,
  },
  [sym_paragraph] = {
    .visible = true,
    .named = true,
  },
  [sym_paragraph_segment] = {
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
  [sym_heading5] = {
    .visible = true,
    .named = true,
  },
  [sym_heading6] = {
    .visible = true,
    .named = true,
  },
  [sym_quote] = {
    .visible = true,
    .named = true,
  },
  [sym_document] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_document_repeat1] = {
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
      ACCEPT_TOKEN(ts_builtin_sym_end);
      if (eof) ADVANCE(1);
      END_STATE();
    case 1:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0, .external_lex_state = 1},
  [1] = {.lex_state = 0, .external_lex_state = 2},
  [2] = {.lex_state = 0, .external_lex_state = 2},
  [3] = {.lex_state = 0, .external_lex_state = 2},
  [4] = {.lex_state = 0},
};

enum {
  ts_external_token_none = 0,
  ts_external_token_paragraph = 1,
  ts_external_token_paragraph_segment = 2,
  ts_external_token_heading1 = 3,
  ts_external_token_heading2 = 4,
  ts_external_token_heading3 = 5,
  ts_external_token_heading4 = 6,
  ts_external_token_heading5 = 7,
  ts_external_token_heading6 = 8,
  ts_external_token_quote = 9,
};

static const TSSymbol ts_external_scanner_symbol_map[EXTERNAL_TOKEN_COUNT] = {
  [ts_external_token_none] = sym_none,
  [ts_external_token_paragraph] = sym_paragraph,
  [ts_external_token_paragraph_segment] = sym_paragraph_segment,
  [ts_external_token_heading1] = sym_heading1,
  [ts_external_token_heading2] = sym_heading2,
  [ts_external_token_heading3] = sym_heading3,
  [ts_external_token_heading4] = sym_heading4,
  [ts_external_token_heading5] = sym_heading5,
  [ts_external_token_heading6] = sym_heading6,
  [ts_external_token_quote] = sym_quote,
};

static const bool ts_external_scanner_states[3][EXTERNAL_TOKEN_COUNT] = {
  [1] = {
    [ts_external_token_none] = true,
    [ts_external_token_paragraph] = true,
    [ts_external_token_paragraph_segment] = true,
    [ts_external_token_heading1] = true,
    [ts_external_token_heading2] = true,
    [ts_external_token_heading3] = true,
    [ts_external_token_heading4] = true,
    [ts_external_token_heading5] = true,
    [ts_external_token_heading6] = true,
    [ts_external_token_quote] = true,
  },
  [2] = {
    [ts_external_token_paragraph] = true,
    [ts_external_token_heading1] = true,
    [ts_external_token_heading2] = true,
    [ts_external_token_heading3] = true,
    [ts_external_token_heading4] = true,
    [ts_external_token_heading5] = true,
    [ts_external_token_heading6] = true,
    [ts_external_token_quote] = true,
  },
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [sym_none] = ACTIONS(1),
    [sym_paragraph] = ACTIONS(1),
    [sym_paragraph_segment] = ACTIONS(1),
    [sym_heading1] = ACTIONS(1),
    [sym_heading2] = ACTIONS(1),
    [sym_heading3] = ACTIONS(1),
    [sym_heading4] = ACTIONS(1),
    [sym_heading5] = ACTIONS(1),
    [sym_heading6] = ACTIONS(1),
    [sym_quote] = ACTIONS(1),
  },
  [1] = {
    [sym_document] = STATE(4),
    [aux_sym_document_repeat1] = STATE(2),
    [sym_paragraph] = ACTIONS(3),
    [sym_heading1] = ACTIONS(3),
    [sym_heading2] = ACTIONS(3),
    [sym_heading3] = ACTIONS(3),
    [sym_heading4] = ACTIONS(3),
    [sym_heading5] = ACTIONS(3),
    [sym_heading6] = ACTIONS(3),
    [sym_quote] = ACTIONS(3),
  },
  [2] = {
    [aux_sym_document_repeat1] = STATE(3),
    [ts_builtin_sym_end] = ACTIONS(5),
    [sym_paragraph] = ACTIONS(7),
    [sym_heading1] = ACTIONS(7),
    [sym_heading2] = ACTIONS(7),
    [sym_heading3] = ACTIONS(7),
    [sym_heading4] = ACTIONS(7),
    [sym_heading5] = ACTIONS(7),
    [sym_heading6] = ACTIONS(7),
    [sym_quote] = ACTIONS(7),
  },
  [3] = {
    [aux_sym_document_repeat1] = STATE(3),
    [ts_builtin_sym_end] = ACTIONS(9),
    [sym_paragraph] = ACTIONS(11),
    [sym_heading1] = ACTIONS(11),
    [sym_heading2] = ACTIONS(11),
    [sym_heading3] = ACTIONS(11),
    [sym_heading4] = ACTIONS(11),
    [sym_heading5] = ACTIONS(11),
    [sym_heading6] = ACTIONS(11),
    [sym_quote] = ACTIONS(11),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 1,
    ACTIONS(14), 1,
      ts_builtin_sym_end,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(4)] = 0,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [5] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_document, 1),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [9] = {.entry = {.count = 1, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 2),
  [11] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_document_repeat1, 2), SHIFT_REPEAT(3),
  [14] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
};

#ifdef __cplusplus
extern "C" {
#endif
void *tree_sitter_norg_external_scanner_create(void);
void tree_sitter_norg_external_scanner_destroy(void *);
bool tree_sitter_norg_external_scanner_scan(void *, TSLexer *, const bool *);
unsigned tree_sitter_norg_external_scanner_serialize(void *, char *);
void tree_sitter_norg_external_scanner_deserialize(void *, const char *, unsigned);

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
    .external_scanner = {
      &ts_external_scanner_states[0][0],
      ts_external_scanner_symbol_map,
      tree_sitter_norg_external_scanner_create,
      tree_sitter_norg_external_scanner_destroy,
      tree_sitter_norg_external_scanner_scan,
      tree_sitter_norg_external_scanner_serialize,
      tree_sitter_norg_external_scanner_deserialize,
    },
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
