module.exports = grammar({
  	name: 'norg',

  	rules: {
    	document: $ => repeat(choice($.quote, $._soft_paragraph_break, $.paragraph, $.marker)),

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

		paragraph: $ => prec.left(1, seq(repeat1(seq(repeat(choice($.heading1, $.heading2, $.heading3, $.heading4)), prec(1, choice($.paragraph_segment, $.quote, $.marker, $.unordered_list, $.todo_item_done, $.todo_item_pending, $.todo_item_undone)))), choice($._eof, $._soft_paragraph_break))),

		words: $ => seq(/[^\s]/, repeat1(/[^\n]/)),
		_eof: $ => token.immediate('\0'),
		_soft_paragraph_break: $ => token.immediate('\n'),
		paragraph_segment: $ => choice(seq($.words, seq($.trailing_modifier, token('\n'), $.paragraph_segment)), seq($.words, $._soft_paragraph_break), seq($.words, $._eof)),
		trailing_modifier: $ => token.immediate('~'),

  	}
});
