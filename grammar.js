module.exports = grammar({
  	name: 'norg',

	conflicts: $ => [
		[$.carryover_tag]
	],

  	rules: {
    	document: $ => repeat(choice($.heading1, $.heading2, $.heading3, $.heading4, $.quote, $.marker, $.tag, $.carryover_tag, $.drawer, $._soft_paragraph_break, $.paragraph)),

		quote: $ => seq(optional($.leading_whitespace), />\s+/, choice($.paragraph_segment, prec(1, seq($.words, $._soft_paragraph_break, $.quote)))),

		heading1: $ => seq(optional($.leading_whitespace), /\*\s+/, $.paragraph_segment),
		heading2: $ => seq(optional($.leading_whitespace), /\*\*\s+/, $.paragraph_segment),
		heading3: $ => seq(optional($.leading_whitespace), /\*\*\*\s+/, $.paragraph_segment),
		heading4: $ => seq(optional($.leading_whitespace), /\*\*\*\*\s+/, $.paragraph_segment),

		unordered_list_prefix: $ => token.immediate(/\-\s+/),
		unordered_list: $ => seq(optional($.leading_whitespace), $.unordered_list_prefix, $.paragraph_segment),
		marker: $ => seq(optional($.leading_whitespace), /\|\s+/, $.paragraph_segment),

		// A bit of a mess, but required for precise syntax highlighting
		todo_item_prefix: $ => token.immediate('['),
		todo_item_suffix: $ => token.immediate(/\]\s+/),
		todo_item_done_mark: $ => token.immediate(/[\t ]*x[\t ]*/),
		todo_item_pending_mark: $ => token.immediate(/[\t ]*\*[\t ]*/),
		todo_item_undone_mark: $ => token.immediate(/\s+/), 

		todo_item_done: $ => seq(optional($.leading_whitespace), $.unordered_list_prefix, $.todo_item_prefix, $.todo_item_done_mark, $.todo_item_suffix, $.paragraph_segment),
		todo_item_pending: $ => seq(optional($.leading_whitespace), $.unordered_list_prefix, $.todo_item_prefix, $.todo_item_pending_mark, $.todo_item_suffix, $.paragraph_segment),
		todo_item_undone: $ => seq(optional($.leading_whitespace), $.unordered_list_prefix, $.todo_item_prefix, $.todo_item_undone_mark, $.todo_item_suffix, $.paragraph_segment),

		tag: $ => seq(optional($.leading_whitespace), token.immediate('@'), $.tag_name, repeat(seq(token.immediate('.'), $.tag_name)), choice(seq(repeat1(token(/[\t ]+/)), choice(seq($.tag_parameters, repeat(seq(token.immediate(/[\t ]+/), $.tag_parameters))), $._soft_paragraph_break)), $._soft_paragraph_break), $.tag_content, $.tag_end),
		tag_content: $ => repeat1(choice(/[^\n]+/, $._soft_paragraph_break)),
		tag_name: $ => repeat1(/[a-z_]/),
		tag_parameters: $ => token.immediate(/[^\n]+/),
		tag_end: $ => token('@end'),

		carryover_tag: $ => seq(optional($.leading_whitespace), token.immediate('$'), $.tag_name, repeat(seq(token.immediate('.'), $.tag_name)), choice(seq(repeat1(token(/[\t ]+/)), choice(seq($.tag_parameters, repeat(seq(token.immediate(/[\t ]+/), $.tag_parameters))), $._soft_paragraph_break)), $._soft_paragraph_break), repeat($._soft_paragraph_break), $.paragraph),

		drawer: $ => seq(optional($.leading_whitespace), token(/\|{2}[\t ]+/), field("drawer_name", $.paragraph_segment), optional($.drawer_content), token('||')),
		drawer_content: $ => repeat1(choice(/[^\\]/, $.escape_sequence)),

		paragraph: $ => prec.left(1, seq(repeat1(prec(1, choice($.paragraph_segment, $.quote, $.unordered_list, $.todo_item_done, $.todo_item_pending, $.todo_item_undone, $.tag, $.drawer, $.heading1, $.heading2, $.heading3, $.heading4, prec(1, $.escape_sequence)))), choice($._soft_paragraph_break, $._eof))),

		_eof: $ => token.immediate('\0'),
		_soft_paragraph_break: $ => token.immediate('\n'),
		leading_whitespace: $ => repeat1(token.immediate(/[\t\v ]/)),

		// Unused, I could not get them to properly get detected with the rules imposed by the Neorg specification
		// If you know how to do this then please consider submitting a PR
		_attached_modifiers: $ => choice(token.immediate('*'), token.immediate('_'), token.immediate('`'), token.immediate('|'), token.immediate('^'), token.immediate('$')),

		escape_sequence: $ => seq(token.immediate('\\'), token.immediate(/./)),
		words: $ => seq(choice(/[^\s]/, $.leading_whitespace), repeat(/[^\n]/)),
		paragraph_segment: $ => choice(seq($.words, $.trailing_modifier, token.immediate('\n'), $.paragraph_segment), seq($.words, choice($._soft_paragraph_break, $._eof))),
		trailing_modifier: $ => token.immediate('~'),

  	}
});
