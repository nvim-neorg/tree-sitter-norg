module.exports = grammar({
  	name: 'norg',

	conflicts: $ => [
		[$.carryover_tag]
	],

  	rules: {
    	document: $ => repeat(choice(prec(2, choice($.quote, $.marker, $.tag, $.carryover_tag, $.drawer)), $._soft_paragraph_break, $.paragraph)),

		quote: $ => seq(/>\s+/, choice($.paragraph_segment, prec(1, seq($.words, $._soft_paragraph_break, $.quote)))),

		heading1: $ => seq(/\*\s+/, $.paragraph_segment),
		heading2: $ => seq(/\*\*\s+/, $.paragraph_segment),
		heading3: $ => seq(/\*\*\*\s+/, $.paragraph_segment),
		heading4: $ => seq(/\*\*\*\*\s+/, $.paragraph_segment),

		unordered_list: $ => seq(/-\s+/, $.paragraph_segment),
		marker: $ => seq(/\|\s+/, $.paragraph_segment),

		todo_item_done: $ => seq(/\-\s+\[\s*x\s*\]\s+/, $.paragraph_segment),
		todo_item_pending: $ => seq(/\-\s+\[\s*\*\s*\]\s+/, $.paragraph_segment),
		todo_item_undone: $ => seq(/\-\s+\[\s+\]\s+/, $.paragraph_segment),

		tag: $ => seq(token(/@/), $.tag_name, repeat(seq(token.immediate('.'), $.tag_name)), choice(seq(/\s+/, seq(choice($.tag_parameters, $._soft_paragraph_break), repeat(seq(/[\t ]+/, $.tag_parameters)), $._soft_paragraph_break)), $._soft_paragraph_break), $.tag_content, '@end'),
		tag_content: $ => prec.left(1, repeat1(seq(/[^\n]+/, $._soft_paragraph_break))),
		tag_name: $ => /[a-z_]+/,
		tag_parameters: $ => /[^\s]+/,

		carryover_tag: $ => seq(token('$'), $.tag_name, repeat(seq(token.immediate('.'), $.tag_name)), choice(seq(/\s+/, seq(choice($.tag_parameters, $._soft_paragraph_break), repeat(seq(/[\t ]+/, $.tag_parameters)), $._soft_paragraph_break)), $._soft_paragraph_break), repeat($._soft_paragraph_break), repeat1($.paragraph)), 

		drawer: $ => seq(/\|{2}\s+/, field("drawer_name", $.paragraph_segment), optional($.drawer_content), '||'),
		drawer_content: $ => /.+/,

		paragraph: $ => prec.left(1, seq(repeat1(seq(repeat(choice($.heading1, $.heading2, $.heading3, $.heading4)), prec(1, choice($.paragraph_segment, $.quote, $.marker, $.unordered_list, $.todo_item_done, $.todo_item_pending, $.todo_item_undone, $.tag, $.drawer)))), choice($._eof, $._soft_paragraph_break))),

		_eof: $ => token.immediate('\0'),
		_soft_paragraph_break: $ => token.immediate('\n'),

		// Unused, I could not get them to properly get detected with the rules imposed by the Neorg specification
		// If you know how to do this then please consider submitting a PR
		_attached_modifiers: $ => choice(token.immediate('*'), token.immediate('_'), token.immediate('`'), token.immediate('|'), token.immediate('^'), token.immediate('$')),

		escape_sequence: $ => seq(token.immediate('\\'), token.immediate(/./)),
		words: $ => seq(/[^\s]/, repeat1(/[^\n]/)),
		paragraph_segment: $ => choice(seq($.words, $.trailing_modifier, token.immediate('\n'), $.paragraph_segment), seq($.words, choice($._soft_paragraph_break, $._eof))),
		trailing_modifier: $ => token.immediate('~'),

  	}
});
