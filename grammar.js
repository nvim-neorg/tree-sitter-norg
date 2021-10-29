/*
* Known bugs:
* INLINE_CODE isn't being placed properly idk why
*/

module.exports = grammar({
    name: 'norg',

    externals: $ => [
 
        $._,
        $.space,

        $.lowercase_word,
        $.capitalized_word,

        $._line_break,
        $._paragraph_break,

        $.escape_sequence_prefix,

        $.heading1_prefix,
        $.heading2_prefix,
        $.heading3_prefix,
        $.heading4_prefix,
        $.heading5_prefix,
        $.heading6_prefix,

        $.quote1_prefix,
        $.quote2_prefix,
        $.quote3_prefix,
        $.quote4_prefix,
        $.quote5_prefix,
        $.quote6_prefix,

        $.unordered_list1_prefix,
        $.unordered_list2_prefix,
        $.unordered_list3_prefix,
        $.unordered_list4_prefix,
        $.unordered_list5_prefix,
        $.unordered_list6_prefix,

        $.ordered_list1_prefix,
        $.ordered_list2_prefix,
        $.ordered_list3_prefix,
        $.ordered_list4_prefix,
        $.ordered_list5_prefix,
        $.ordered_list6_prefix,

        $.marker_prefix,

        $.todo_item_undone,
        $.todo_item_pending,
        $.todo_item_done,

        $.insertion_prefix,

        $.unordered_link1_prefix,
        $.unordered_link2_prefix,
        $.unordered_link3_prefix,
        $.unordered_link4_prefix,
        $.unordered_link5_prefix,
        $.unordered_link6_prefix,

        $.ordered_link1_prefix,
        $.ordered_link2_prefix,
        $.ordered_link3_prefix,
        $.ordered_link4_prefix,
        $.ordered_link5_prefix,
        $.ordered_link6_prefix,

        $.strong_paragraph_delimiter,
        $.weak_paragraph_delimiter,
        $.horizontal_line,

        $.link_text_prefix,
        $.text,
        $.link_text_suffix,

        $.link_location_prefix,
        $.link_file_begin,
        $.link_file_location,
        $.link_file_end,

        $.link_end_generic,
        $.link_end_url,
        $.link_end_external_file,
        $.link_end_heading1_reference,
        $.link_end_heading2_reference,
        $.link_end_heading3_reference,
        $.link_end_heading4_reference,
        $.link_end_heading5_reference,
        $.link_end_heading6_reference,
        $.link_end_marker_reference,
        $.link_location_suffix,

        $.ranged_tag_prefix,
        $.ranged_tag_end_prefix,

        $.carryover_tag_prefix,

        $.single_definition_prefix,
        $.multi_definition_prefix,
        $.multi_definition_suffix,

        $.bold_segment,
        $.italic_segment,
        $.strikethrough_segment,
        $.underline_segment,
        $.spoiler_segment,
        $.verbatim,
        $.superscript_segment,
        $.subscript_segment,
        $.inline_comment_segment,

        $.bold_segment_with_nest,
        $.italic_segment_with_nest,
        $.strikethrough_segment_with_nest,
        $.underline_segment_with_nest,
        $.spoiler_segment_with_nest,
        $.verbatim_with_nest,
        $.superscript_segment_with_nest,
        $.subscript_segment_with_nest,
        $.inline_comment_segment_with_nest,

        $.markup_end,
      ],

      rules: {
        document: $ =>
            seq(
                optional($.foreplay),
                optional($.document_content),
            ),

        foreplay: $ =>
            prec.right(0,
                repeat1(
                    prec(2,
                        choice(
                            $._paragraph_break,
                            $._line_break,
                            $.ranged_tag,
                            $.insertion,
                        )
                    ),
                )
            ),

        document_content: $ => repeat1(
            choice(
                prec(1,
                    choice(
                        $._paragraph_break,
                        $._line_break,
                        $._heading,
                        $._detached_modifier,
                        $._definition,
                        $._tag,
                        $.horizontal_line,
                        // Markers are separate from detached modifiers because they are the a l p h a modifier (consumes all elements)
                        $.marker,
                    )
                ),

                $.paragraph,
                $.strong_paragraph_delimiter,
            )
        ),

        word: $ =>
            choice(
                alias($.lowercase_word, "_lowercase"),
                alias($.capitalized_word, "_uppercase"),
            ),

        // Any regular text
        _paragraph: $ =>
            prec.right(0,
                seq(
                    $.paragraph,

                    optional(
                        $._paragraph_break,
                    )
                )
            ),

        paragraph_segment: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        alias($.word, "_word"),
                        alias($.space, "_space"),
                        $.link,
                        $.escape_sequence,
                    ),
                )
            ),

        paragraph: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.paragraph_segment,
                        $._line_break,

                        $._attached_modifier,
                    )
                )
            ),

        // ---- ATTACHED MODIFIERS ----
        bold: $ =>
            prec.right(0, 
                repeat1(
                    choice(
                        $.bold_segment,
                        seq(
                            $.bold_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.italic,
                                        $.strikethrough,
                                        $.underline,
                                        $.spoiler,
                                        $.superscript,
                                        $.subscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        italic: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.italic_segment,
                        seq(
                            $.italic_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.strikethrough,
                                        $.underline,
                                        $.spoiler,
                                        $.superscript,
                                        $.subscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        strikethrough: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.strikethrough_segment,
                        seq(
                            $.strikethrough_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.italic,
                                        $.underline,
                                        $.spoiler,
                                        $.superscript,
                                        $.subscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        underline: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.underline_segment,
                        seq(
                            $.underline_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.italic,
                                        $.strikethrough,
                                        $.spoiler,
                                        $.superscript,
                                        $.subscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        spoiler: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.spoiler_segment,
                        seq(
                            $.spoiler_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.italic,
                                        $.strikethrough,
                                        $.underline,
                                        $.superscript,
                                        $.subscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        superscript: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.superscript_segment,
                        seq(
                            $.superscript_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.italic,
                                        $.strikethrough,
                                        $.underline,
                                        $.spoiler,
                                        $.subscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        subscript: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.subscript_segment,
                        seq(
                            $.subscript_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.italic,
                                        $.strikethrough,
                                        $.underline,
                                        $.spoiler,
                                        $.superscript,
                                        $.inline_comment,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        inline_comment: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.inline_comment_segment,
                        seq(
                            $.inline_comment_segment_with_nest,
                            seq(
                                repeat1(
                                    choice(
                                        $.bold,
                                        $.italic,
                                        $.strikethrough,
                                        $.underline,
                                        $.spoiler,
                                        $.superscript,
                                        $.subscript,
                                    ),
                                ),
                                $.markup_end,
                            )
                        ),
                    ),
                ),
            ),

        // Well, any character
        any_char: $ =>
            token.immediate(/./),

        // A backslash followed by the escape token (e.g. \*)
        escape_sequence: $ =>
            seq(
                $.escape_sequence_prefix,

                field(
                    "token",
                    $.any_char,
                )
            ),

        link_text: $ =>
            seq(
                alias($.link_text_prefix, "_prefix"),
                field("link_text", optional($.text)),
                alias($.link_text_suffix, "_suffix"),
            ),

        link_file: $ =>
            seq(
                alias($.link_file_begin, "_prefix"),
                field("location", $.link_file_location),
                alias($.link_file_end, "_suffix"),
            ),
        
        link_end: $ =>
            seq(
                field("type", choice(
                    $.link_end_url,
                    $.link_end_generic,
                    $.link_end_external_file,
                    $.link_end_marker_reference,
                    $.link_end_heading1_reference,
                    $.link_end_heading2_reference,
                    $.link_end_heading3_reference,
                    $.link_end_heading4_reference,
                    $.link_end_heading5_reference,
                    $.link_end_heading6_reference,
                )),
                $.text,
            ),

        link_location: $ =>
            seq(
                alias($.link_location_prefix, "_prefix"),
                choice(
                    seq(
                        $.link_file,
                        optional(
                            $.link_end,
                        ),
                    ),
                    $.link_end,
                ),
                alias($.link_location_suffix, "_suffix"),
            ),

        link: $ =>
            seq(
                $.link_text,
                optional($.link_location),
            ),

        strict_link: $ =>
            seq(
                $.link_text,
                $.link_location,
            ),

        unordered_link1: $ =>
            prec.right(0,
                seq(
                    $.unordered_link1_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break
                    ),

                    repeat(
                        choice(
                            $.unordered_link2,
                            $.unordered_link3,
                            $.unordered_link4,
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link2,
                            $.ordered_link3,
                            $.ordered_link4,
                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        unordered_link2: $ =>
            prec.right(0,
                seq(
                    $.unordered_link2_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break
                    ),


                    repeat(
                        choice(
                            $.unordered_link3,
                            $.unordered_link4,
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link3,
                            $.ordered_link4,
                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        unordered_link3: $ =>
            prec.right(0,
                seq(
                    $.unordered_link3_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),
                    optional(
                        $._line_break
                    ),


                    repeat(
                        choice(
                            $.unordered_link4,
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link4,
                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        unordered_link4: $ =>
            prec.right(0,
                seq(
                    $.unordered_link4_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),
                    optional(
                        $._line_break
                    ),


                    repeat(
                        choice(
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        unordered_link5: $ =>
            prec.right(0,
                seq(
                    $.unordered_link5_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),
                    optional(
                        $._line_break
                    ),


                    repeat(
                    	choice(
                        	$.unordered_link6,
                        	$.ordered_link6,
                    	)
                    )
                )
            ),

        unordered_link6: $ =>
            prec.right(0,
                seq(
                    $.unordered_link6_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break
                    ),
                ),
            ),

        ordered_link1: $ =>
            prec.right(0,
                seq(
                    $.ordered_link1_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break,
                    ),

                    repeat(
                        choice(
                            $.unordered_link2,
                            $.unordered_link3,
                            $.unordered_link4,
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link2,
                            $.ordered_link3,
                            $.ordered_link4,
                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        ordered_link2: $ =>
            prec.right(0,
                seq(
                    $.ordered_link2_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break,
                    ),

                    repeat(
                        choice(
                            $.unordered_link3,
                            $.unordered_link4,
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link3,
                            $.ordered_link4,
                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        ordered_link3: $ =>
            prec.right(0,
                seq(
                    $.ordered_link3_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break,
                    ),

                    repeat(
                        choice(
                            $.unordered_link4,
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link4,
                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        ordered_link4: $ =>
            prec.right(0,
                seq(
                    $.ordered_link4_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break,
                    ),

                    repeat(
                        choice(
                            $.unordered_link5,
                            $.unordered_link6,

                            $.ordered_link5,
                            $.ordered_link6,
                        )
                    )
                )
            ),

        ordered_link5: $ =>
            prec.right(0,
                seq(
                    $.ordered_link5_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break,
                    ),

                    repeat(
                        choice(
                        	$.unordered_link6,
                        	$.ordered_link6,
                    	)
                    )
                )
            ),

        ordered_link6: $ =>
            prec.right(0,
                seq(
                    $.ordered_link6_prefix,

                    field(
                        "location",
                        $.strict_link,
                    ),

                    optional(
                        $._line_break,
                    ),
                )
            ),

        // A first-level heading:
        // * Example
        heading1: $ =>
            prec.right(0,
                seq(
                    $.heading1_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "content",

                        repeat(
                            choice(
                                $._paragraph,

                                $._paragraph_break,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $.horizontal_line,

                                $.heading2,
                                $.heading3,
                                $.heading4,
                                $.heading5,
                                $.heading6,
                            )
                        )
                    ),

                    optional(
                        $.weak_paragraph_delimiter,
                    )
                )
            ),

        // A second-level heading:
        // ** Example
        heading2: $ =>
            prec.right(0,
                seq(
                    $.heading2_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "content",

                        repeat(
                            choice(
                                $._paragraph,

                                $._paragraph_break,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $.horizontal_line,

                                $.heading3,
                                $.heading4,
                                $.heading5,
                                $.heading6,
                            )
                        )
                    ),

                    optional(
                        $.weak_paragraph_delimiter,
                    )
                )
            ),

        // A third-level heading:
        // *** Example
        heading3: $ =>
            prec.right(0,
                seq(
                    $.heading3_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "content",

                        repeat(
                            choice(
                                $._paragraph,

                                $._paragraph_break,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $.horizontal_line,

                                $.heading4,
                                $.heading5,
                                $.heading6,
                            )
                        )
                    ),

                    optional(
                        $.weak_paragraph_delimiter,
                    )
                )
            ),

        // A fourth-level heading:
        // **** Example
        heading4: $ =>
            prec.right(0,
                seq(
                    $.heading4_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "content",

                        repeat(
                            choice(
                                $._paragraph,

                                $._paragraph_break,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $.horizontal_line,

                                $.heading5,
                                $.heading6,
                            )
                        )
                    ),

                    optional(
                        $.weak_paragraph_delimiter,
                    )
                )
            ),

        // A fifth-level heading:
        // ***** Example
        heading5: $ =>
            prec.right(0,
                seq(
                    $.heading5_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "content",

                        repeat(
                            choice(
                                $._paragraph,

                                $._paragraph_break,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $.horizontal_line,

                                $.heading6,
                            )
                        )
                    ),

                    optional(
                        $.weak_paragraph_delimiter,
                    )
                )
            ),

        // A sixth-level heading:
        // ******* Example
        heading6: $ =>
            prec.right(0,
                seq(
                    $.heading6_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "content",

                        repeat(
                            choice(
                                $._paragraph,

                                $._paragraph_break,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $.horizontal_line,
                            )
                        )
                    ),

                    optional(
                        $.weak_paragraph_delimiter,
                    )
                )
            ),

        // A quote:
        // > That's what she said
        quote: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.quote1,
                        $.quote2,
                        $.quote3,
                        $.quote4,
                        $.quote5,
                        $.quote6
                    )
                )
            ),

        quote1: $ =>
            prec.right(0,
                seq(
                    $.quote1_prefix,

                    field(
                        "content",
                        $.paragraph_segment,
                    ),

                    repeat(
                        choice(
                            $.quote2,
                            $.quote3,
                            $.quote4,
                            $.quote5,
                            $.quote6,
                        ),
                    )
                )
            ),

        quote2: $ =>
            prec.right(0,
                seq(
                    $.quote2_prefix,

                    field(
                        "content",
                        $.paragraph_segment,
                    ),

                    repeat(
                        choice(
                            $.quote3,
                            $.quote4,
                            $.quote5,
                            $.quote6,
                        ),
                    )
                )
            ),

        quote3: $ =>
            prec.right(0,
                seq(
                    $.quote3_prefix,

                    field(
                        "content",
                        $.paragraph_segment,
                    ),

                    repeat(
                        choice(
                            $.quote4,
                            $.quote5,
                            $.quote6,
                        ),
                    )
                )
            ),

        quote4: $ =>
            prec.right(0,
                seq(
                    $.quote4_prefix,

                    field(
                        "content",
                        $.paragraph_segment,
                    ),

                    repeat(
                        choice(
                            $.quote5,
                            $.quote6,
                        ),
                    )
                )
            ),

        quote5: $ =>
            prec.right(0,
                seq(
                    $.quote5_prefix,

                    field(
                        "content",
                        $.paragraph_segment,
                    ),

                    repeat(
                        $.quote6,
                    )
                )
            ),

        quote6: $ =>
            prec.right(0,
                seq(
                    $.quote6_prefix,

                    field(
                        "content",
                        $.paragraph_segment,
                    )
                )
            ),

        // TODO: complete docs tomorrow
        generic_list: $ =>
            prec.right(0,
                repeat1(
                    choice(
                        $.unordered_list1,
                        $.unordered_list2,
                        $.unordered_list3,
                        $.unordered_list4,
                        $.unordered_list5,
                        $.unordered_list6,

                        $.ordered_list1,
                        $.ordered_list2,
                        $.ordered_list3,
                        $.ordered_list4,
                        $.ordered_list5,
                        $.ordered_list6,

                        $.todo_item1,
                        $.todo_item2,
                        $.todo_item3,
                        $.todo_item4,
                        $.todo_item5,
                        $.todo_item6,

						$.unordered_link1,
						$.unordered_link2,
						$.unordered_link3,
						$.unordered_link4,
						$.unordered_link5,
						$.unordered_link6,

						$.ordered_link1,
						$.ordered_link2,
						$.ordered_link3,
						$.ordered_link4,
						$.ordered_link5,
						$.ordered_link6,
                    )
                )
            ),

        unordered_list1: $ =>
            prec.right(0,
                seq(
                    $.unordered_list1_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),

                    repeat(
                        choice(
                            $.todo_item2,
                            $.todo_item3,
                            $.todo_item4,
                            $.todo_item5,
                            $.todo_item6,

                            $.unordered_list2,
                            $.unordered_list3,
                            $.unordered_list4,
                            $.unordered_list5,
                            $.unordered_list6,

                            $.ordered_list2,
                            $.ordered_list3,
                            $.ordered_list4,
                            $.ordered_list5,
                            $.ordered_list6,
                        )
                    )
                )
            ),

        unordered_list2: $ =>
            prec.right(0,
                seq(
                    $.unordered_list2_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),

                    repeat(
                        choice(
                            $.todo_item3,
                            $.todo_item4,
                            $.todo_item5,
                            $.todo_item6,

                            $.unordered_list3,
                            $.unordered_list4,
                            $.unordered_list5,
                            $.unordered_list6,

                            $.ordered_list3,
                            $.ordered_list4,
                            $.ordered_list5,
                            $.ordered_list6,
                        )
                    )
                )
            ),

        unordered_list3: $ =>
            prec.right(0,
                seq(
                    $.unordered_list3_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),

                    repeat(
                        choice(
                            $.todo_item4,
                            $.todo_item5,
                            $.todo_item6,

                            $.unordered_list4,
                            $.unordered_list5,
                            $.unordered_list6,

                            $.ordered_list4,
                            $.ordered_list5,
                            $.ordered_list6,
                        )
                    )
                )
            ),

        unordered_list4: $ =>
            prec.right(0,
                seq(
                    $.unordered_list4_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),

                    repeat(
                        choice(
                            $.todo_item5,
                            $.todo_item6,

                            $.unordered_list5,
                            $.unordered_list6,

                            $.ordered_list5,
                            $.ordered_list6,
                        )
                    )
                )
            ),

        unordered_list5: $ =>
            prec.right(0,
                seq(
                    $.unordered_list5_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),

                    repeat(
                        choice(
                            $.todo_item6,
                            $.unordered_list6,
                            $.ordered_list6,
                        )
                    )
                )
            ),

        unordered_list6: $ =>
            prec.right(0,
                seq(
                    $.unordered_list6_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),
                )
            ),

        ordered_list1: $ =>
            prec.right(0,
              seq(
                  $.ordered_list1_prefix,

                  field(
                      "content",
                      $.paragraph,
                  ),

                  repeat(
                      choice(
                          $.todo_item2,
                          $.todo_item3,
                          $.todo_item4,
                          $.todo_item5,
                          $.todo_item6,

                          $.unordered_list2,
                          $.unordered_list3,
                          $.unordered_list4,
                          $.unordered_list5,
                          $.unordered_list6,

                          $.ordered_list2,
                          $.ordered_list3,
                          $.ordered_list4,
                          $.ordered_list5,
                          $.ordered_list6,
                      )
                  )
              )
          ),

        ordered_list2: $ =>
            prec.right(0,
              seq(
                  $.ordered_list2_prefix,

                  field(
                      "content",
                      $.paragraph,
                  ),

                  repeat(
                      choice(
                          $.todo_item3,
                          $.todo_item4,
                          $.todo_item5,
                          $.todo_item6,

                          $.unordered_list3,
                          $.unordered_list4,
                          $.unordered_list5,
                          $.unordered_list6,

                          $.ordered_list3,
                          $.ordered_list4,
                          $.ordered_list5,
                          $.ordered_list6,
                      )
                  )
              )
          ),

        ordered_list3: $ =>
            prec.right(0,
              seq(
                  $.ordered_list3_prefix,

                  field(
                      "content",
                      $.paragraph,
                  ),

                  repeat(
                      choice(
                          $.todo_item4,
                          $.todo_item5,
                          $.todo_item6,

                          $.unordered_list4,
                          $.unordered_list5,
                          $.unordered_list6,

                          $.ordered_list4,
                          $.ordered_list5,
                          $.ordered_list6,
                      )
                  )
              )
          ),

        ordered_list4: $ =>
            prec.right(0,
              seq(
                  $.ordered_list4_prefix,

                  field(
                      "content",
                      $.paragraph,
                  ),

                  repeat(
                      choice(
                          $.todo_item5,
                          $.todo_item6,

                          $.unordered_list5,
                          $.unordered_list6,

                          $.ordered_list5,
                          $.ordered_list6,
                      )
                  )
              )
          ),

        ordered_list5: $ =>
            prec.right(0,
              seq(
                  $.ordered_list5_prefix,

                  field(
                      "content",
                      $.paragraph,
                  ),

                  repeat(
                      choice(
                          $.todo_item6,
                          $.unordered_list6,
                          $.ordered_list6,
                      )
                  )
              )
          ),

        ordered_list6: $ =>
            prec.right(0,
                seq(
                    $.ordered_list6_prefix,

                    field(
                        "content",
                        $.paragraph,
                    ),
                )
            ),

        marker: $ =>
            prec.right(0,
                seq(
                    $.marker_prefix,

                    field(
                        "name",
                        $.paragraph_segment
                    ),

                    repeat(prec(1, $._line_break)),

                    field(
                        "subtext",
                        repeat(
                            choice(
                                $._paragraph,
                                $.strong_paragraph_delimiter,
                                $.horizontal_line,
                                $._heading,
                                $._detached_modifier,
                                $._definition,
                                $._tag,
                                $._paragraph_break,
            ),
                    ),
                            )
                        )
                    ),

        // --------------------------------------------------
        todo_item1: $ =>
            prec.right(0,
                seq(
                    $.unordered_list1_prefix,

                    field(
                        "state",

                        choice(
                            $.todo_item_undone,
                            $.todo_item_pending,
                            $.todo_item_done,
                        ),
                    ),

                    token.immediate(/\s+/),

                    field(
                        "content",
                        $.paragraph
                    ),

                    repeat(
                        choice(
                            $.todo_item2,
                            $.todo_item3,
                            $.todo_item4,
                            $.todo_item5,
                            $.todo_item6,
                        ),
                    ),
                )
            ),

        todo_item2: $ =>
            prec.right(0,
                seq(
                    $.unordered_list2_prefix,

                    field(
                        "state",

                        choice(
                            $.todo_item_undone,
                            $.todo_item_pending,
                            $.todo_item_done,
                        )
                    ),

                    token(/\s+/),

                    field(
                        "content",
                        $.paragraph
                    ),

                    repeat(
                        choice(
                            $.todo_item3,
                            $.todo_item4,
                            $.todo_item5,
                            $.todo_item6,
                        ),
                    ),
                )
            ),

        todo_item3: $ =>
            prec.right(0,
                seq(
                    $.unordered_list3_prefix,

                    field(
                        "state",

                        choice(
                            $.todo_item_undone,
                            $.todo_item_pending,
                            $.todo_item_done,
                        )
                    ),

                    token(/\s+/),

                    field(
                        "content",
                        $.paragraph
                    ),

                    repeat(
                        choice(
                            $.todo_item4,
                            $.todo_item5,
                            $.todo_item6,
                        ),
                    ),
                )
            ),

        todo_item4: $ =>
            prec.right(0,
                seq(
                    $.unordered_list4_prefix,

                    field(
                        "state",

                        choice(
                            $.todo_item_undone,
                            $.todo_item_pending,
                            $.todo_item_done,
                        )
                    ),

                    token(/\s+/),

                    field(
                        "content",
                        $.paragraph
                    ),

                    repeat(
                        choice(
                            $.todo_item5,
                            $.todo_item6,
                        )
                    ),
                )
            ),

        todo_item5: $ =>
            prec.right(0,
                seq(
                    $.unordered_list5_prefix,

                    field(
                        "state",

                        choice(
                            $.todo_item_undone,
                            $.todo_item_pending,
                            $.todo_item_done,
                        )
                    ),

                    token(/\s+/),

                    field(
                        "content",
                        $.paragraph
                    ),

                    repeat(
                        $.todo_item6,
                    ),
                )
            ),

        todo_item6: $ =>
            prec.right(0,
                seq(
                    $.unordered_list6_prefix,

                    field(
                        "state",

                        choice(
                            $.todo_item_undone,
                            $.todo_item_pending,
                            $.todo_item_done,
                        )
                    ),

                    token(/\s+/),

                    field(
                        "content",
                        $.paragraph
                    )
                )
            ),

        insertion: $ =>
            prec.right(0,
                seq(
                    $.insertion_prefix,

                    field(
                        "item",
                        choice(
                            $.capitalized_word,
                            $.lowercase_word
                        )
                    ),

                    choice(
                        seq(
                            token.immediate(
                                /[\t\v ]+/
                            ),
                            field(
                                "parameters",
                                $.paragraph_segment
                            )
                        ),

                        token.immediate(
                            /[\t\v ]*\n/
                        ),
                    ),
                )
            ),

        single_definition: $ =>
            seq(
                $.single_definition_prefix,

                field(
                    "title",
                    $.paragraph_segment
                ),

                field(
                    "definition",
                    $.paragraph
                ),
            ),

        multi_definition: $ =>
            choice(
                seq(
                    $.multi_definition_prefix,

                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    field(
                        "content",
                        repeat(
                            choice(
                                $._paragraph,
                                $._paragraph_break,

                                $._detached_modifier,
                                $._tag,
                            )
                        )
                    ),

                    // Used for preventing annoying errors with incomplete marker definitions
                    field(
                        "end",
                        $.multi_definition_suffix
                    ),
                ),

                alias(
                    $.multi_definition_suffix,
                    "_suffix"
                )
            ),

        // TODO: Comment scanner code
        ranged_tag_content: $ =>
            prec.right(0, 
                repeat1(
                    choice(
                        alias(
                            choice(
                                $.paragraph_segment,
                                $._line_break,
                                $._paragraph_break,
                            ),
                            "_segment",
                        ),

                        field(
                            "nested_tag",
                            $.ranged_tag
                        ),
                    ),
                ),
            ),

        ranged_tag_end: $ =>
            seq(
                alias(
                    $.ranged_tag_end_prefix,
                    "_prefix",
                ),

                alias(
                    token.immediate("end"),
                    "_name",
                ),
            ),

        ranged_tag: $ =>
            prec.right(0,
                seq(
                    alias(
                        $.ranged_tag_prefix,
                        "_prefix"
                    ),

                    field(
                        "name",
                        $.tag_name,
                    ),

                    choice(
                        token.immediate(
                            /[\t\v ]*\n/,
                        ),

                        seq(
                            token.immediate(
                                /[\t\v ]+/,
                            ),

                            $.tag_parameters,

                            token.immediate(
                                '\n'
                            ),
                        ),
                    ),

                    field(
                        "content",
                        optional(
                            $.ranged_tag_content,
                        ),
                    ),

                    optional($.ranged_tag_end),
                ),
            ),

        carryover_tag_set: $ =>
            prec.left(0,
                seq(
                    repeat1(
                        $.carryover_tag,
                    ),

                    field(
                        "target",

                        choice(
                            $._paragraph,
                            repeat1(
                                choice(
                                    $._detached_modifier,
                                    $._definition,
                                    $._heading,
                                    $.ranged_tag,
                                    $.marker,
                                ),
                            ),
                        ),
                    ),
                )
            ),

        carryover_tag: $ =>
            seq(
                alias(
                    $.carryover_tag_prefix,
                    "_prefix",
                ),

                field(
                    "name",
                    $.tag_name,
                ),

                choice(
                    token.immediate(
                        /[\t\v ]*\n/,
                    ),

                    seq(
                        token.immediate(
                            /[\t\v ]+/,
                        ),

                        $.tag_parameters,

                        token.immediate(
                            '\n'
                        ),
                    ),
                ),

                repeat(
                    $._paragraph_break,
                ),
            ),

        tag_name_element: $ =>
            seq(
                token(/[a-z0-9_\-\+]+/),
                token(/[a-zA-Z0-9_\-\+]*/),
            ),

        tag_name: $ =>
            seq(
                choice(
                    $.tag_name_element,
                ),

                repeat(
                    seq(
                        alias(
                            token.immediate("."),
                            "_delimiter",
                        ),

                        $.tag_name_element,
                    )
                )
            ),

        tag_parameters: $ =>
            seq(
                field(
                    "parameter",
                    $.word
                ),

                repeat(
                    seq(
                        token.immediate(/[\t\v ]+/),

                        field(
                            "parameter",
                            optional(
                                $.word
                            ),
                        ),
                    )
                ),
            ),

        _tag: $ =>
            choice(
                $.ranged_tag,
                $.carryover_tag_set,
            ),

        // --------------------------------------------------

        _heading: $ =>
            choice(
                $.heading1,
                $.heading2,
                $.heading3,
                $.heading4,
                $.heading5,
                $.heading6,
            ),

        // definitions are in a separate group and not part of _detached_modifier
        // because this allows us to nest the _detached_modifier group within
        // multi-paragraph definitions (but not nest definitions themselves)
        _definition: $ =>
            choice(
                $.single_definition,
                $.multi_definition,
            ),

        // A list of detached modifiers (excluding headings and definitons)
        _detached_modifier: $ =>
            choice(
                $.quote,
                $.generic_list,
                $.insertion,
            ),

        _attached_modifier_segment: $ =>
            choice(
                $.bold_segment,
                $.italic_segment,
                $.strikethrough_segment,
                $.underline_segment,
                $.spoiler_segment,
                $.superscript_segment,
                $.subscript_segment,
                $.markup_end,
            ),

        _attached_modifier: $ =>
            choice(
                $.bold,
                $.italic,
                $.strikethrough,
                $.underline,
                $.verbatim,
                $.spoiler,
                $.superscript,
                $.subscript,
            ),
      }
});
