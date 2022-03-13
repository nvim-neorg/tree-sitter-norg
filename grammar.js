module.exports = grammar({
    name: 'norg',

    supertypes: $ => [
        $.attached_modifier,
        $.heading,
        $.detached_modifier,
        $.footnote,
        $.definition,
        $.tag,
    ],

    conflicts: $ => [
        [$.bold, $._conflict_open],
        [$.italic, $._conflict_open],
        [$.strikethrough, $._conflict_open],
        [$.underline, $._conflict_open],
        [$.spoiler, $._conflict_open],
        [$.verbatim, $._conflict_open],
        [$.superscript, $._conflict_open],
        [$.subscript, $._conflict_open],
        [$.inline_comment, $._conflict_open],
        [$.inline_math, $._conflict_open],
        [$.variable, $._conflict_open],

        [$.bold, $._paragraph_element],
        [$.italic, $._paragraph_element],
        [$.strikethrough, $._paragraph_element],
        [$.underline, $._paragraph_element],
        [$.spoiler, $._paragraph_element],
        [$.verbatim, $._paragraph_element],
        [$.superscript, $._paragraph_element],
        [$.subscript, $._paragraph_element],
        [$.inline_comment, $._paragraph_element],
        [$.inline_math, $._paragraph_element],
        [$.variable, $._paragraph_element],

        [$.unordered_link1, $.paragraph],
        [$.unordered_link2, $.paragraph],
        [$.unordered_link3, $.paragraph],
        [$.unordered_link4, $.paragraph],
        [$.unordered_link5, $.paragraph],
        [$.unordered_link6, $.paragraph],
        [$.ordered_link1, $.paragraph],
        [$.ordered_link2, $.paragraph],
        [$.ordered_link3, $.paragraph],
        [$.ordered_link4, $.paragraph],
        [$.ordered_link5, $.paragraph],
        [$.ordered_link6, $.paragraph],

        [$._paragraph_element],
    ],

    externals: $ => [
        $._,
        $.space,

        $.lowercase_word,
        $.capitalized_word,

        $.line_break,
        $.paragraph_break,

        $.escape_sequence_prefix,

        $.trailing_modifier,

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
        $.todo_item_on_hold,
        $.todo_item_cancelled,
        $.todo_item_urgent,
        $.todo_item_uncertain,
        $.todo_item_recurring,

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

        $.link_description_begin,
        $.link_description_end,
        $.link_location_begin,
        $.link_location_end,
        $.link_file_begin,
        $.link_file_end,
        $.link_file_text,
        $.link_target_url,
        $.link_target_generic,
        $.link_target_external_file,
        $.link_target_marker,
        $.link_target_definition,
        $.link_target_footnote,
        $.link_target_heading1,
        $.link_target_heading2,
        $.link_target_heading3,
        $.link_target_heading4,
        $.link_target_heading5,
        $.link_target_heading6,

        $.ranged_tag_prefix,
        $.ranged_tag_end_prefix,

        $.carryover_tag_prefix,

        $.single_definition_prefix,
        $.multi_definition_prefix,
        $.multi_definition_suffix,

        $.single_footnote_prefix,
        $.multi_footnote_prefix,
        $.multi_footnote_suffix,

        $.link_modifier,

        $.bold_open,
        $.bold_close,

        $.italic_open,
        $.italic_close,

        $.strikethrough_open,
        $.strikethrough_close,

        $.underline_open,
        $.underline_close,

        $.spoiler_open,
        $.spoiler_close,

        $.superscript_open,
        $.superscript_close,

        $.subscript_open,
        $.subscript_close,

        $.verbatim_open,
        $.verbatim_close,

        $.inline_comment_open,
        $.inline_comment_close,

        $.inline_math_open,
        $.inline_math_close,

        $.variable_open,
        $.variable_close,
    ],

    rules: {
        document: $ => repeat1(
            choice(
                prec(1,
                    choice(
                        alias($.paragraph_break, "_paragraph_break"),
                        alias($.line_break, "_line_break"),
                        $.heading,
                        $.detached_modifier,
                        $.definition,
                        $.footnote,
                        $.tag,
                        $.horizontal_line,
                        $.strong_paragraph_delimiter,
                        // Markers are separate from detached modifiers because they are the a l p h a modifier (consumes all elements)
                        $.marker,
                    )
                ),

                $.paragraph,
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
                    alias($.paragraph_break, "_paragraph_break"),
                )
            )
        ),

        _paragraph_element: $ =>
        choice(
            alias($.word, "_word"),
            alias($.space, "_space"),
            alias($.trailing_modifier, "_trailing_modifier"),
            $.link,
            $.anchor_declaration,
            $.anchor_definition,
            $.escape_sequence,
            seq(
                optional($.link_modifier),
                $.attached_modifier,
                optional($.link_modifier),
            ),
            alias($.link_modifier, "_word"),
            alias($.bold_close, "_word"),
            alias($.italic_close, "_word"),
            alias($.strikethrough_close, "_word"),
            alias($.underline_close, "_word"),
            alias($.spoiler_close, "_word"),
            alias($.superscript_close, "_word"),
            alias($.subscript_close, "_word"),
            alias($.verbatim_close, "_word"),
            alias($.inline_comment_close, "_word"),
            alias($.inline_math_close, "_word"),
            alias($.variable_close, "_word"),
        ),

        _multi_paragraph_element: $ =>
        repeat1(
            choice(
                alias($.line_break, "_line_break"),
                $._paragraph_element,
                $._conflict_open,
            ),
        ),

        _verbatim_segment: $ =>
        prec.left(1,
            seq(
                repeat1(
                    choice(
                        alias($.line_break, "_line_break"),
                        alias($.word, "_word"),
                        alias($.space, "_space"),
                        alias($.trailing_modifier, "_word"),
                        alias($.escape_sequence_prefix, "_word"),
                        alias($.any_char, "_word"),
                        alias($.link_modifier, "_word"),
                        alias($.bold_open, "_word"),
                        alias($.bold_close, "_word"),
                        alias($.italic_open, "_word"),
                        alias($.italic_close, "_word"),
                        alias($.strikethrough_open, "_word"),
                        alias($.strikethrough_close, "_word"),
                        alias($.underline_open, "_word"),
                        alias($.underline_close, "_word"),
                        alias($.spoiler_open, "_word"),
                        alias($.spoiler_close, "_word"),
                        alias($.superscript_open, "_word"),
                        alias($.superscript_close, "_word"),
                        alias($.subscript_open, "_word"),
                        alias($.subscript_close, "_word"),
                        alias($.verbatim_open, "_word"),
                        alias($.verbatim_close, "_word"),
                        alias($.inline_comment_open, "_word"),
                        alias($.inline_comment_close, "_word"),
                        alias($.inline_math_open, "_word"),
                        alias($.inline_math_close, "_word"),
                        alias($.variable_open, "_word"),
                        alias($.variable_close, "_word"),
                        alias($.link_description_begin, "_word"),
                        alias($.link_description_end, "_word"),
                        alias($.link_location_begin, "_word"),
                        alias($.link_location_end, "_word"),
                        alias($.link_file_begin, "_word"),
                        alias($.link_file_end, "_word"),
                        alias($.link_file_text, "_word"),
                        alias($.link_target_url, "_word"),
                        alias($.link_target_generic, "_word"),
                        alias($.link_target_external_file, "_word"),
                        alias($.link_target_marker, "_word"),
                        alias($.link_target_definition, "_word"),
                        alias($.link_target_footnote, "_word"),
                        alias($.link_target_heading1, "_word"),
                        alias($.link_target_heading2, "_word"),
                        alias($.link_target_heading3, "_word"),
                        alias($.link_target_heading4, "_word"),
                        alias($.link_target_heading5, "_word"),
                        alias($.link_target_heading6, "_word"),
                    ),
                ),
            ),
        ),

        paragraph_segment: $ =>
        prec.right(0,
            repeat1(
                choice(
                    $._paragraph_element,
                    alias($._conflict_open, "_word"),
                ),
            )
        ),

        paragraph: $ =>
        prec.right(0,
            repeat1(
                choice(
                    $.paragraph_segment,
                    alias($.line_break, "_line_break"),
                )
            ),
        ),

        // ---- ATTACHED MODIFIERS ----
        bold: $ =>
        seq(
            alias($.bold_open, "_open"),
            $._multi_paragraph_element,
            alias($.bold_close, "_close"),
        ),

        italic: $ =>
        seq(
            alias($.italic_open, "_open"),
            $._multi_paragraph_element,
            alias($.italic_close, "_close"),
        ),

        strikethrough: $ =>
        seq(
            alias($.strikethrough_open, "_open"),
            $._multi_paragraph_element,
            alias($.strikethrough_close, "_close"),
        ),

        underline: $ =>
        seq(
            alias($.underline_open, "_open"),
            $._multi_paragraph_element,
            alias($.underline_close, "_close"),
        ),

        spoiler: $ =>
        seq(
            alias($.spoiler_open, "_open"),
            $._multi_paragraph_element,
            alias($.spoiler_close, "_close"),
        ),

        verbatim: $ =>
        seq(
            alias($.verbatim_open, "_open"),
            $._verbatim_segment,
            alias($.verbatim_close, "_close"),
        ),

        superscript: $ =>
        seq(
            alias($.superscript_open, "_open"),
            $._multi_paragraph_element,
            alias($.superscript_close, "_close"),
        ),

        subscript: $ =>
        seq(
            alias($.subscript_open, "_open"),
            $._multi_paragraph_element,
            alias($.subscript_close, "_close"),
        ),

        inline_comment: $ =>
        seq(
            alias($.inline_comment_open, "_open"),
            $._verbatim_segment,
            alias($.inline_comment_close, "_close"),
        ),

        inline_math: $ =>
        seq(
            alias($.inline_math_open, "_open"),
            $._verbatim_segment,
            alias($.inline_math_close, "_close"),
        ),

        variable: $ =>
        seq(
            alias($.variable_open, "_open"),
            $._verbatim_segment,
            alias($.variable_close, "_close"),
        ),

        _conflict_open: $ =>
        prec.dynamic(-1,
            choice(
                alias($.bold_open, "_word"),
                alias($.italic_open, "_word"),
                alias($.strikethrough_open, "_word"),
                alias($.underline_open, "_word"),
                alias($.spoiler_open, "_word"),
                alias($.verbatim_open, "_word"),
                alias($.superscript_open, "_word"),
                alias($.subscript_open, "_word"),
                alias($.inline_comment_open, "_word"),
                alias($.inline_math_open, "_word"),
                alias($.variable_open, "_word"),
            )
        ),

        // Well, any character
        any_char: _ =>
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

        link_description: $ =>
        seq(
            alias($.link_description_begin, "_begin"),
            field("text", $.paragraph_segment),
            alias($.link_description_end, "_end"),
        ),

        link_location: $ =>
        seq(
            alias($.link_location_begin, "_begin"),
            choice(
                $._link_target,
                seq(
                    $._link_file,
                    optional(
                        $._link_target,
                    ),
                ),
            ),
            alias($.link_location_end, "_end"),
        ),

        _link_file: $ =>
        seq(
            alias($.link_file_begin, "_begin"),
            field("file", $.link_file_text),
            alias($.link_file_end, "_end"),
        ),

        _link_target: $ =>
        choice(
            $._link_target_markup,
            $._link_target_verbatim,
        ),

        _link_target_markup: $ =>
        seq(
            field("type",
                choice(
                    $.link_target_generic,
                    $.link_target_external_file,
                    $.link_target_marker,
                    $.link_target_definition,
                    $.link_target_footnote,
                    $.link_target_heading1,
                    $.link_target_heading2,
                    $.link_target_heading3,
                    $.link_target_heading4,
                    $.link_target_heading5,
                    $.link_target_heading6,
                ),
            ),
            field("text", $.paragraph_segment),
        ),

        _link_target_verbatim: $ =>
        seq(
            field("type",
                choice(
                    $.link_target_url,
                ),
            ),
            field("text", alias($._verbatim_segment, $.paragraph_segment)),
        ),

        link: $ =>
        prec.right(2, seq(
            $.link_location,
            optional(
                $.link_description,
            ),
        )),

        anchor_declaration: $ => $.link_description,

        anchor_definition: $ =>
        prec(2, seq(
            $.link_description,
            $.link_location,
        )),

        unordered_link1: $ =>
        prec.right(0,
            seq(
                $.unordered_link1_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_2,
                )
            )
        ),

        unordered_link2: $ =>
        prec.right(0,
            seq(
                $.unordered_link2_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_3,
                )
            )
        ),

        unordered_link3: $ =>
        prec.right(0,
            seq(
                $.unordered_link3_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_4,
                )
            )
        ),

        unordered_link4: $ =>
        prec.right(0,
            seq(
                $.unordered_link4_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_5,
                )
            )
        ),

        unordered_link5: $ =>
        prec.right(0,
            seq(
                $.unordered_link5_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_6,
                )
            )
        ),

        unordered_link6: $ =>
        prec.right(0,
            seq(
                $.unordered_link6_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),
            ),
        ),

        ordered_link1: $ =>
        prec.right(0,
            seq(
                $.ordered_link1_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_2,
                )
            )
        ),

        ordered_link2: $ =>
        prec.right(0,
            seq(
                $.ordered_link2_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_3,
                )
            )
        ),

        ordered_link3: $ =>
        prec.right(0,
            seq(
                $.ordered_link3_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_4,
                )
            )
        ),

        ordered_link4: $ =>
        prec.right(0,
            seq(
                $.ordered_link4_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_5,
                )
            )
        ),

        ordered_link5: $ =>
        prec.right(0,
            seq(
                $.ordered_link5_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    $._any_list_item_level_6,
                )
            )
        ),

        ordered_link6: $ =>
        prec.right(0,
            seq(
                $.ordered_link6_prefix,

                field(
                    "location",
                    choice(
                        $.link,
                        $.anchor_definition,
                    ),
                ),

                optional(
                    choice(
                        alias($.line_break, "_line_break"),
                        field("description", $.paragraph),
                    )
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

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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

                optional(prec(1, alias($.line_break, "_line_break"))),

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

                optional(prec(1, alias($.line_break, "_line_break"))),

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

                optional(prec(1, alias($.line_break, "_line_break"))),

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

                optional(prec(1, alias($.line_break, "_line_break"))),

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

                optional(prec(1, alias($.line_break, "_line_break"))),

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
                ),

                optional(prec(1, alias($.line_break, "_line_break"))),

            )
        ),

        _any_list_item_level_1: $ =>
        choice(
            $.unordered_list1,
            $.ordered_list1,
            $.todo_item1,
            $.unordered_link1,
            $.ordered_link1,
            $._any_list_item_level_2,
        ),

        _any_list_item_level_2: $ =>
        choice(
            $.unordered_list2,
            $.ordered_list2,
            $.todo_item2,
            $.unordered_link2,
            $.ordered_link2,
            $._any_list_item_level_3,
        ),

        _any_list_item_level_3: $ =>
        choice(
            $.unordered_list3,
            $.ordered_list3,
            $.todo_item3,
            $.unordered_link3,
            $.ordered_link3,
            $._any_list_item_level_4,
        ),

        _any_list_item_level_4: $ =>
        choice(
            $.unordered_list4,
            $.ordered_list4,
            $.todo_item4,
            $.unordered_link4,
            $.ordered_link4,
            $._any_list_item_level_5,
        ),

        _any_list_item_level_5: $ =>
        choice(
            $.unordered_list5,
            $.ordered_list5,
            $.todo_item5,
            $.unordered_link5,
            $.ordered_link5,
            $._any_list_item_level_6,
        ),

        _any_list_item_level_6: $ =>
        choice(
            $.unordered_list6,
            $.ordered_list6,
            $.todo_item6,
            $.unordered_link6,
            $.ordered_link6,
        ),

        // TODO: complete docs
        generic_list: $ =>
        prec.right(0,
            repeat1(
                $._any_list_item_level_1,
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
                    $._any_list_item_level_2,
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
                    $._any_list_item_level_3,
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
                    $._any_list_item_level_4,
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
                    $._any_list_item_level_5,
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
                    $._any_list_item_level_6,
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
                    $._any_list_item_level_2,
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
                    $._any_list_item_level_3,
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
                    $._any_list_item_level_4,
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
                    $._any_list_item_level_5,
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
                    $._any_list_item_level_6,
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
                    "title",
                    $.paragraph_segment
                ),

                repeat(prec(1, alias($.line_break, "_line_break"))),

                field(
                    "subtext",
                    repeat(
                        choice(
                            $._paragraph,
                            $.strong_paragraph_delimiter,
                            $.horizontal_line,
                            $.heading,
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
                            alias($.paragraph_break, "_paragraph_break"),
                        ),
                    ),
                )
            )
        ),

        // --------------------------------------------------
        _any_todo_state: $ =>
        choice(
            $.todo_item_undone,
            $.todo_item_pending,
            $.todo_item_done,
            $.todo_item_on_hold,
            $.todo_item_cancelled,
            $.todo_item_urgent,
            $.todo_item_uncertain,
            $.todo_item_recurring,
        ),

        todo_item1: $ =>
        prec.right(0,
            seq(
                $.unordered_list1_prefix,

                field(
                    "state",
                    $._any_todo_state,
                ),

                token.immediate(/\s+/),

                field(
                    "content",
                    $.paragraph
                ),

                repeat(
                    $._any_list_item_level_2,
                ),
            )
        ),

        todo_item2: $ =>
        prec.right(0,
            seq(
                $.unordered_list2_prefix,

                field(
                    "state",
                    $._any_todo_state,
                ),

                token(/\s+/),

                field(
                    "content",
                    $.paragraph
                ),

                repeat(
                    $._any_list_item_level_3,
                ),
            )
        ),

        todo_item3: $ =>
        prec.right(0,
            seq(
                $.unordered_list3_prefix,

                field(
                    "state",
                    $._any_todo_state,
                ),

                token(/\s+/),

                field(
                    "content",
                    $.paragraph
                ),

                repeat(
                    $._any_list_item_level_4,
                ),
            )
        ),

        todo_item4: $ =>
        prec.right(0,
            seq(
                $.unordered_list4_prefix,

                field(
                    "state",
                    $._any_todo_state,
                ),

                token(/\s+/),

                field(
                    "content",
                    $.paragraph
                ),

                repeat(
                    $._any_list_item_level_5,
                ),
            )
        ),

        todo_item5: $ =>
        prec.right(0,
            seq(
                $.unordered_list5_prefix,

                field(
                    "state",
                    $._any_todo_state,
                ),

                token(/\s+/),

                field(
                    "content",
                    $.paragraph
                ),

                repeat(
                    $._any_list_item_level_6,
                ),
            )
        ),

        todo_item6: $ =>
        prec.right(0,
            seq(
                $.unordered_list6_prefix,

                field(
                    "state",
                    $._any_todo_state,
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
        prec.right(seq(
            $.single_definition_prefix,

            field(
                "title",
                $.paragraph_segment
            ),

            repeat(
                prec(1, choice(
                    alias($.line_break, "_line_break"),
                    alias($.paragraph_break, "_paragraph_break"),
                )),
            ),

            field(
                "definition",
                $.paragraph
            ),
        )),

        multi_definition: $ =>
        choice(
            seq(
                $.multi_definition_prefix,

                field(
                    "title",
                    $.paragraph_segment,
                ),

                choice(
                    alias($.line_break, "_line_break"),
                    alias($.paragraph_break, "_paragraph_break"),
                ),

                field(
                    "content",
                    repeat(
                        choice(
                            $._paragraph,
                            alias($.paragraph_break, "_paragraph_break"),

                            $.detached_modifier,
                            $.tag,
                        )
                    )
                ),

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

        single_footnote: $ =>
        prec.right(seq(
            $.single_footnote_prefix,

            field(
                "title",
                $.paragraph_segment
            ),

            repeat(
                prec(1, choice(
                    alias($.line_break, "_line_break"),
                    alias($.paragraph_break, "_paragraph_break"),
                )),
            ),

            field(
                "content",
                $.paragraph
            ),
        )),

        multi_footnote: $ =>
        choice(
            seq(
                $.multi_footnote_prefix,

                field(
                    "title",
                    $.paragraph_segment,
                ),

                choice(
                    alias($.line_break, "_line_break"),
                    alias($.paragraph_break, "_paragraph_break"),
                ),

                field(
                    "content",
                    repeat(
                        choice(
                            $._paragraph,
                            alias($.paragraph_break, "_paragraph_break"),

                            $.detached_modifier,
                            $.tag,
                        )
                    )
                ),

                field(
                    "end",
                    $.multi_footnote_suffix,
                ),
            ),

            alias(
                $.multi_footnote_suffix,
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
                            alias($.line_break, "_line_break"),
                            alias($.paragraph_break, "_paragraph_break"),
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
                                $.detached_modifier,
                                $.definition,
                                $.footnote,
                                $.heading,
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
                alias($.paragraph_break, "_paragraph_break"),
            ),
        ),

        tag_name_element: _ =>
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

        tag_param: $ => /\S+/,

        tag_parameters: $ =>
        seq(
            field(
                "parameter",
                $.tag_param,
            ),

            repeat(
                seq(
                    token.immediate(/[\t\v ]+/),

                    field(
                        "parameter",
                        optional(
                            $.tag_param,
                        ),
                    ),
                )
            ),
        ),

        tag: $ =>
        choice(
            $.ranged_tag,
            $.carryover_tag_set,
        ),

        // --------------------------------------------------

        heading: $ =>
        choice(
            $.heading1,
            $.heading2,
            $.heading3,
            $.heading4,
            $.heading5,
            $.heading6,
        ),

        // definitions are in a separate group and not part of detached_modifier
        // because this allows us to nest the detached_modifier group within
        // multi-paragraph definitions (but not nest definitions themselves)
        definition: $ =>
        choice(
            $.single_definition,
            $.multi_definition,
        ),

        footnote: $ =>
        choice(
            $.single_footnote,
            $.multi_footnote,
        ),

        // A list of detached modifiers (excluding headings and definitons)
        detached_modifier: $ =>
        choice(
            $.quote,
            $.generic_list,
            $.insertion,
        ),

        attached_modifier: $ =>
        choice(
            $.bold,
            $.italic,
            $.strikethrough,
            $.underline,
            $.verbatim,
            $.spoiler,
            $.superscript,
            $.subscript,
            $.inline_comment,
            $.inline_math,
            $.variable,
        ),
    }
});
