module.exports = grammar({
  	name: 'norg',

  	rules: {
    	document: $ => repeat(choice($.quote, $._soft_paragraph_break, $.paragraph_segment, $.heading1)),

		quote: $ => seq(/>\s+/, choice($.paragraph_segment, prec(1, seq($.words, $._soft_paragraph_break, $.quote)))),

		heading1: $ => prec.left(1, seq(/\*\s+/, $.paragraph_segment, optional(seq(repeat1($.paragraph_segment), choice('\n\n', $._eof))))),

		words: $ => seq(/[^\s]/, repeat1(/[^\n]/)),
		_eof: $ => token.immediate('\0'),
		_soft_paragraph_break: $ => token.immediate('\n'),
		paragraph_segment: $ => choice(seq($.words, seq($.trailing_modifier, token('\n'), $.paragraph_segment)), seq($.words, $._soft_paragraph_break), seq($.words, $._eof)),
		trailing_modifier: $ => token.immediate('~'),

  	}
});
