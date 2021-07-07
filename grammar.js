module.exports = grammar({
  	name: 'norg',

	conflicts: $ => [
		[$.carryover_tag]
	],

  	rules: {
    	document: $ => repeat(choice($.quote, $.marker, $.tag, $.carryover_tag, $.drawer, $._soft_paragraph_break, $.paragraph)),

		quote: $ => seq(optional($._leading_whitespace), />\s+/, choice($.paragraph_segment, prec(1, seq($.words, $._soft_paragraph_break, $.quote)))),

		heading1: $ => seq(optional($._leading_whitespace), /\*\s+/, $.paragraph_segment),
		heading2: $ => seq(optional($._leading_whitespace), /\*\*\s+/, $.paragraph_segment),
		heading3: $ => seq(optional($._leading_whitespace), /\*\*\*\s+/, $.paragraph_segment),
		heading4: $ => seq(optional($._leading_whitespace), /\*\*\*\*\s+/, $.paragraph_segment),

		unordered_list: $ => seq(optional($._leading_whitespace), /-\s+/, $.paragraph_segment),
		marker: $ => seq(optional($._leading_whitespace), /\|\s+/, $.paragraph_segment),

		todo_item_prefix: $ => token.immediate(/\-\s+\[/),
		todo_item_appendix: $ => token.immediate(/\]\s+/),
		_todo_item_done_mark: $ => token.immediate(/[\t ]*x[\t ]*/),
		_todo_item_pending_mark: $ => token.immediate(/[\t ]*\*[\t ]*/),

		todo_item_done: $ => seq(optional($._leading_whitespace), $.todo_item_prefix, $._todo_item_done_mark, $.todo_item_appendix, $.paragraph_segment),
		todo_item_pending: $ => seq(optional($._leading_whitespace), $.todo_item_prefix, $._todo_item_pending_mark, $.todo_item_appendix, $.paragraph_segment),
		todo_item_undone: $ => seq(optional($._leading_whitespace), $.todo_item_prefix, token.immediate(/\s+/), $.todo_item_appendix, $.paragraph_segment),

		tag: $ => seq(optional($._leading_whitespace), token.immediate('@'), $.tag_name, repeat(seq(token.immediate(/\./), $.tag_name)), choice(seq(repeat1(token(/[\t ]/)), choice($.tag_parameters, $._soft_paragraph_break), repeat(seq(/[\t ]+/, $.tag_parameters)), $._soft_paragraph_break), $._soft_paragraph_break), optional($.tag_content), '@end'),
		tag_content: $ => repeat1(seq(/[^\n]+/, $._soft_paragraph_break)),
		tag_name: $ => repeat1(/[a-z_]/),
		tag_parameters: $ => /[^\n]+/,

		carryover_tag: $ => seq(optional($._leading_whitespace), token('$'), $.tag_name, repeat(seq(token.immediate('.'), $.tag_name)), choice(seq(/\s+/, seq(choice($.tag_parameters, $._soft_paragraph_break), repeat(seq(/[\t ]+/, $.tag_parameters)), $._soft_paragraph_break)), $._soft_paragraph_break), repeat($._soft_paragraph_break), $.paragraph), 

		drawer: $ => seq(optional($._leading_whitespace), token(/\|{2}[\t ]+/), field("drawer_name", $.paragraph_segment), optional($.drawer_content), token('||')),
		drawer_content: $ => repeat1(choice(/[^\\]/, $.escape_sequence)),

		paragraph: $ => prec.left(1, seq(repeat1(seq(repeat(choice($.heading1, $.heading2, $.heading3, $.heading4)), prec(1, choice($.paragraph_segment, $.quote, $.marker, $.unordered_list, $.todo_item_done, $.todo_item_pending, $.todo_item_undone, $.tag, $.drawer, prec(1, $.escape_sequence))))), choice($._eof, $._soft_paragraph_break))),

		_eof: $ => token.immediate('\0'),
		_soft_paragraph_break: $ => token.immediate('\n'),
		_leading_whitespace: $ => repeat1(token.immediate(/[\t\v ]/)),

		// Unused, I could not get them to properly get detected with the rules imposed by the Neorg specification
		// If you know how to do this then please consider submitting a PR
		_attached_modifiers: $ => choice(token.immediate('*'), token.immediate('_'), token.immediate('`'), token.immediate('|'), token.immediate('^'), token.immediate('$')),

		escape_sequence: $ => seq(token.immediate('\\'), token.immediate(/./)),
		words: $ => seq(choice(/[^\s]/, $._leading_whitespace), repeat(/[^\n]/)),
		paragraph_segment: $ => choice(seq($.words, $.trailing_modifier, token.immediate('\n'), $.paragraph_segment), seq($.words, choice($._soft_paragraph_break, $._eof))),
		trailing_modifier: $ => token.immediate('~'),

  	}
});
