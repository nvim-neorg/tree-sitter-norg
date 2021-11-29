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
    ],

    externals: $ => [
        $._,
        $.space,

        $.lowercase_word,
        $.capitalized_word,

        $._line_break,
        $._paragraph_break,

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

        $.link_begin,
        $.link_file_begin,
        $.link_file_text,
        $.link_file_end,
        $.link_location_generic,
        $.link_location_url,
        $.link_location_external_file,
        $.link_location_heading1,
        $.link_location_heading2,
        $.link_location_heading3,
        $.link_location_heading4,
        $.link_location_heading5,
        $.link_location_heading6,
        $.link_location_marker,
        $.link_location_definition,
        $.link_location_footnote,
        $.link_location_text,
        $.link_end,
        $.link_text_begin,
        $.link_text,
        $.link_text_end,

        $.anchor_declaration_begin,
        $.anchor_declaration_text,
        $.anchor_declaration_end,

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
        $.italic_open,
        $.strikethrough_open,
        $.underline_open,
        $.spoiler_open,
        $.verbatim_open,
        $.superscript_open,
        $.subscript_open,
        $.inline_comment_open,
        $.inline_math_open,
        $.variable_open,

        $.markup_close,
    ],

    rules: {
        document: $ => repeat1(
            choice(
                prec(1,
                    choice(
                        $._paragraph_break,
                        $._line_break,
                        $.heading,
                        $.detached_modifier,
                        $.definition,
                        $.footnote,
                        $.tag,
                        $.horizontal_line,
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
                    $._paragraph_break,
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
            $.link_modifier,
            $.attached_modifier,
        ),

        _multi_paragraph_element: $ =>
        repeat1(
            choice(
                $._line_break,
                $._paragraph_element,
            ),
        ),

        _verbatim_segment: $ =>
        seq(
            repeat1(
                prec.right(1,
                    choice(
                        $._paragraph_element,
                        alias($.bold_open, "_lowercase"),
                        alias($.italic_open, "_lowercase"),
                        alias($.strikethrough_open, "_lowercase"),
                        alias($.underline_open, "_lowercase"),
                        alias($.spoiler_open, "_lowercase"),
                        alias($.verbatim_open, "_lowercase"),
                        alias($.superscript_open, "_lowercase"),
                        alias($.subscript_open, "_lowercase"),
                        alias($.inline_comment_open, "_lowercase"),
                        alias($.inline_math_open, "_lowercase"),
                        alias($.variable_open, "_lowercase"),
                    ),
                )
            ),
        ),

        paragraph_segment: $ =>
        prec.right(0,
            repeat1(
                choice(
                    $._paragraph_element,
                    alias($._conflict_open, "_lowercase"),
                    alias($.markup_close, "_lowercase"),
                ),
            )
        ),

        paragraph: $ =>
        prec.right(0,
            repeat1(
                choice(
                    $.paragraph_segment,
                    $._line_break,
                )
            ),
        ),

        // ---- ATTACHED MODIFIERS ----
        bold: $ =>
        seq(
            alias($.bold_open, "_open_bold"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        italic: $ =>
        seq(
            alias($.italic_open, "_open_italic"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        strikethrough: $ =>
        seq(
            alias($.strikethrough_open, "_open_strikethrough"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        underline: $ =>
        seq(
            alias($.underline_open, "_open_underline"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        spoiler: $ =>
        seq(
            alias($.spoiler_open, "_open_spoiler"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        verbatim: $ =>
        seq(
            alias($.verbatim_open, "_open_verbatim"),
            $._verbatim_segment,
            alias($.markup_close, "_close"),
        ),

        superscript: $ =>
        seq(
            alias($.superscript_open, "_open_superscript"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        subscript: $ =>
        seq(
            alias($.subscript_open, "_open_subscript"),
            $._multi_paragraph_element,
            alias($.markup_close, "_close"),
        ),

        inline_comment: $ =>
        seq(
            alias($.inline_comment_open, "_open_inline_comment"),
            $._verbatim_segment,
            alias($.markup_close, "_close"),
        ),

        inline_math: $ =>
        seq(
            alias($.inline_math_open, "_open_inline_math"),
            $._verbatim_segment,
            alias($.markup_close, "_close"),
        ),

        variable: $ =>
        seq(
            alias($.variable_open, "_open_variable"),
            $._verbatim_segment,
            alias($.markup_close, "_close"),
        ),

        _conflict_open: $ =>
        prec.dynamic(-1,
            choice(
                alias($.bold_open, "_lowercase"),
                alias($.italic_open, "_lowercase"),
                alias($.strikethrough_open, "_lowercase"),
                alias($.underline_open, "_lowercase"),
                alias($.spoiler_open, "_lowercase"),
                alias($.verbatim_open, "_lowercase"),
                alias($.superscript_open, "_lowercase"),
                alias($.subscript_open, "_lowercase"),
                alias($.inline_comment_open, "_lowercase"),
                alias($.inline_math_open, "_lowercase"),
                alias($.variable_open, "_lowercase"),
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
            alias($.link_text_begin, "_begin"),
            field("text", $.link_text),
            alias($.link_text_end, "_end"),
        ),

        link_file: $ =>
        seq(
            alias($.link_file_begin, "_begin"),
            field("location", $.link_file_text),
            alias($.link_file_end, "_end"),
        ),

        link_location: $ =>
        seq(
            field("type", choice(
                $.link_location_generic,
                $.link_location_url,
                $.link_location_external_file,
                $.link_location_heading1,
                $.link_location_heading2,
                $.link_location_heading3,
                $.link_location_heading4,
                $.link_location_heading5,
                $.link_location_heading6,
                $.link_location_marker,
                $.link_location_definition,
                $.link_location_footnote,
            )),
            field("text", $.link_location_text),
        ),


        link: $ =>
        seq(
            alias($.link_begin, "_begin"),
            choice(
                $.link_location,
                seq(
                    $.link_file,
                    optional(
                        $.link_location,
                    )
                )
            ),
            alias($.link_end, "_end"),
            optional(
                $.link_description,
            )
        ),

        anchor_declaration: $ =>
        seq(
            alias($.anchor_declaration_begin, "_begin"),
            field("text", $.anchor_declaration_text),
            alias($.anchor_declaration_end, "_end"),
        ),

        anchor_definition: $ =>
        prec(2, seq(
            $.anchor_declaration,
            alias($.link_begin, "_begin"),
            choice(
                $.link_location,
                seq(
                    $.link_file,
                    optional(
                        $.link_location,
                    )
                )
            ),
            alias($.link_end, "_end"),
        )),

        unordered_link1: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
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

        unordered_link2: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
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

        unordered_link3: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    choice(
                        $.unordered_link4,
                        $.unordered_link5,
                        $.unordered_link6,

                        $.ordered_link4,
                        $.ordered_link5,
                        $.ordered_link6,

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

        unordered_link4: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    choice(
                        $.unordered_link5,
                        $.unordered_link6,

                        $.ordered_link5,
                        $.ordered_link6,

                        $.unordered_list5,
                        $.unordered_list6,

                        $.ordered_list5,
                        $.ordered_list6,
                    )
                )
            )
        ),

        unordered_link5: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    choice(
                        $.unordered_link6,
                        $.ordered_link6,

                        $.unordered_list6,
                        $.ordered_list6,
                    )
                )
            )
        ),

        unordered_link6: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),
            ),
        ),

        ordered_link1: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
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

        ordered_link2: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
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

        ordered_link3: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    choice(
                        $.unordered_link4,
                        $.unordered_link5,
                        $.unordered_link6,

                        $.ordered_link4,
                        $.ordered_link5,
                        $.ordered_link6,

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

        ordered_link4: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    choice(
                        $.unordered_link5,
                        $.unordered_link6,

                        $.ordered_link5,
                        $.ordered_link6,

                        $.unordered_list5,
                        $.unordered_list6,

                        $.ordered_list5,
                        $.ordered_list6,
                    )
                )
            )
        ),

        ordered_link5: $ =>
        prec.right(1,
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
                        $._line_break,
                        field("description", $.paragraph),
                    )
                ),

                repeat(
                    choice(
                        $.unordered_link6,
                        $.ordered_link6,

                        $.unordered_list6,
                        $.ordered_list6,
                    )
                )
            )
        ),

        ordered_link6: $ =>
        prec.right(1,
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
                        $._line_break,
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

                repeat(prec(1, $._line_break)),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            $._paragraph_break,
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
                    choice(
                        $.weak_paragraph_delimiter,
                        $.strong_paragraph_delimiter,
                    )
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

                repeat(prec(1, $._line_break)),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            $._paragraph_break,
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

                repeat(prec(1, $._line_break)),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            $._paragraph_break,
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

                repeat(prec(1, $._line_break)),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            $._paragraph_break,
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

                repeat(prec(1, $._line_break)),

                field(
                    "content",

                    repeat(
                        choice(
                            $._paragraph,

                            $._paragraph_break,
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

                optional(prec(1, $._line_break)),

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

                optional(prec(1, $._line_break)),

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

                optional(prec(1, $._line_break)),

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

                optional(prec(1, $._line_break)),

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

                optional(prec(1, $._line_break)),

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

                optional(prec(1, $._line_break)),

            )
        ),

        // TODO: complete docs
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

                        $.unordered_link5,
                        $.unordered_link6,

                        $.ordered_link5,
                        $.ordered_link6,
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
                        $.unordered_link6,
                        $.ordered_link6,
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

                        $.unordered_link5,
                        $.unordered_link6,

                        $.ordered_link5,
                        $.ordered_link6,
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
                        $.unordered_link6,
                        $.ordered_link6,
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
                    "title",
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
                            $.heading,
                            $.detached_modifier,
                            $.definition,
                            $.footnote,
                            $.tag,
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
                        $.todo_item_on_hold,
                        $.todo_item_cancelled,
                        $.todo_item_urgent,
                        $.todo_item_uncertain,
                        $.todo_item_recurring,
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
                        $.todo_item_on_hold,
                        $.todo_item_cancelled,
                        $.todo_item_urgent,
                        $.todo_item_uncertain,
                        $.todo_item_recurring,
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
                        $.todo_item_on_hold,
                        $.todo_item_cancelled,
                        $.todo_item_urgent,
                        $.todo_item_uncertain,
                        $.todo_item_recurring,
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
                        $.todo_item_on_hold,
                        $.todo_item_cancelled,
                        $.todo_item_urgent,
                        $.todo_item_uncertain,
                        $.todo_item_recurring,
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
                        $.todo_item_on_hold,
                        $.todo_item_cancelled,
                        $.todo_item_urgent,
                        $.todo_item_uncertain,
                        $.todo_item_recurring,
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
                        $.todo_item_on_hold,
                        $.todo_item_cancelled,
                        $.todo_item_urgent,
                        $.todo_item_uncertain,
                        $.todo_item_recurring,
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
        seq(
            $.single_footnote_prefix,

            field(
                "title",
                $.paragraph_segment
            ),

            field(
                "content",
                $.paragraph
            ),
        ),

        multi_footnote: $ =>
        choice(
            seq(
                $.multi_footnote_prefix,

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
                $._paragraph_break,
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
