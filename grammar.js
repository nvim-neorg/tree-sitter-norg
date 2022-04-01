module.exports = grammar({
    name: 'norg',

    inline: $ => [
        $.attached_modifier,
        $.heading,
        $.detached_modifier,
        $.footnote,
        $.definition,
        $.tag,
        $.any_todo_state,
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

        [$._paragraph_element],
        [$.indent_segment],
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

        $.todo_item_undone,
        $.todo_item_pending,
        $.todo_item_done,
        $.todo_item_on_hold,
        $.todo_item_cancelled,
        $.todo_item_urgent,
        $.todo_item_uncertain,
        $.todo_item_recurring,

        $.marker_prefix,

        $.insertion_prefix,

        $.single_definition_prefix,
        $.multi_definition_prefix,
        $.multi_definition_suffix,

        $.single_footnote_prefix,
        $.multi_footnote_prefix,
        $.multi_footnote_suffix,

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
        $.ranged_verbatim_tag_prefix,
        $.ranged_verbatim_tag_end_prefix,

        $.carryover_tag_prefix,

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

        $.inline_link_target_open,
        $.inline_link_target_close,

        $.indent_segment_begin,
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

        // Any word
        word: $ =>
        choice(
            alias($.lowercase_word, "_lowercase"),
            alias($.capitalized_word, "_uppercase"),
        ),

        // Any regular text. A paragraph is made up of `paragraph_segment`
        // objects and line breaks. It may optionally be followed by a paragraph
        // break (which are two consecutive line breaks).
        paragraph: $ =>
        prec.right(0,
            seq(
                repeat1(
                    choice(
                        $.paragraph_segment,
                        alias($.line_break, "_line_break"),
                    ),
                ),
            ),
        ),

        // A paragraph segment can contain any paragraph element.
        paragraph_segment: $ =>
        prec.right(0,
            repeat1(
                choice(
                    $._paragraph_element,
                    alias($._conflict_open, "_word"),
                ),
            )
        ),

        // The attached modifiers cannot contain a `paragraph_segment` directly,
        // because they:
        //   - require a higher precedence of their internals
        _attached_modifier_content: $ =>
        prec.right(1,
            repeat1(
                choice(
                    $._paragraph_element,
                    alias($.line_break, "_line_break"),
                ),
            ),
        ),

        // Same as the non-verbatim modifier contents but using verbatim
        // elements instead.
        _verbatim_modifier_content: $ =>
        prec.right(1,
            repeat1(
                choice(
                    $._verbatim_paragraph_element,
                    alias($.line_break, "_line_break"),
                ),
            ),
        ),

        // Any of the following choices are valid IN-LINE elements. Any
        // multitude of these are combined to form a `paragraph_segment`.
        _paragraph_element: $ =>
        choice(
            alias($.word, "_word"),
            alias($.space, "_space"),
            alias($.trailing_modifier, "_trailing_modifier"),
            $.link,
            $.anchor_declaration,
            $.anchor_definition,
            $.inline_link_target,
            $.escape_sequence,
            seq(
                optional($.link_modifier),
                $.attached_modifier,
                optional($.link_modifier),
            ),
            alias($.link_modifier, "_word"),
            alias($._conflict_close, "_word"),
        ),

        // A verbatim paragraph element essentially ignores all inline markup.
        _verbatim_paragraph_element: $ =>
        choice(
            alias($.word, "_word"),
            alias($.space, "_space"),
            alias($.trailing_modifier, "_word"),
            alias($.escape_sequence_prefix, "_word"),
            alias($.any_char, "_word"),
            alias($.link_modifier, "_word"),
            prec.dynamic(5, alias($._conflict_open, "_word")),
            prec.dynamic(5, alias($._conflict_close, "_word")),
            alias($.inline_link_target_open, "_word"),
            alias($.inline_link_target_close, "_word"),
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

        // ---- ATTACHED MODIFIERS ----
        bold: $ => gen_attached_modifier($, "bold", false),
        italic: $ => gen_attached_modifier($, "italic", false),
        strikethrough: $ => gen_attached_modifier($, "strikethrough", false),
        underline: $ => gen_attached_modifier($, "underline", false),
        spoiler: $ => gen_attached_modifier($, "spoiler", false),
        superscript: $ => gen_attached_modifier($, "superscript", false),
        subscript: $ => gen_attached_modifier($, "subscript", false),
        inline_comment: $ => gen_attached_modifier($, "inline_comment", false),
        verbatim: $ => gen_attached_modifier($, "verbatim", true),
        inline_math: $ => gen_attached_modifier($, "inline_math", true),
        variable: $ => gen_attached_modifier($, "variable", true),

        _conflict_open: $ =>
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
        ),

        _conflict_close: $ =>
        choice(
            alias($.bold_close, "_word"),
            alias($.italic_close, "_word"),
            alias($.strikethrough_close, "_word"),
            alias($.underline_close, "_word"),
            alias($.spoiler_close, "_word"),
            alias($.verbatim_close, "_word"),
            alias($.superscript_close, "_word"),
            alias($.subscript_close, "_word"),
            alias($.inline_comment_close, "_word"),
            alias($.inline_math_close, "_word"),
            alias($.variable_close, "_word"),
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

        _link_content: $ =>
        prec.right(1,
            seq(
                repeat1(
                    choice(
                        $._paragraph_element,
                        alias($._conflict_open, "_word"),
                        alias($.line_break, "_line_break"),
                    ),
                ),
            ),
        ),

        _link_verbatim_content: $ =>
        prec.right(1,
            seq(
                repeat1(
                    $._verbatim_modifier_content,
                ),
            ),
        ),

        inline_link_target: $ =>
        seq(
            alias($.inline_link_target_open, "_open"),
            field("text", alias($._link_content, $.paragraph)),
            alias($.inline_link_target_close, "_close"),
        ),

        link_description: $ =>
        seq(
            alias($.link_description_begin, "_begin"),
            field("text", alias($._link_content, $.paragraph)),
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
            field("text", alias($._link_content, $.paragraph)),
        ),

        _link_target_verbatim: $ =>
        seq(
            field("type",
                choice(
                    $.link_target_url,
                    $.link_target_external_file,
                ),
            ),
            field("text", alias($._link_verbatim_content, $.paragraph)),
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

        // A first-level heading:
        // * Example
        heading1: $ => gen_heading($, 1),
        heading2: $ => gen_heading($, 2),
        heading3: $ => gen_heading($, 3),
        heading4: $ => gen_heading($, 4),
        heading5: $ => gen_heading($, 5),
        heading6: $ => gen_heading($, 6),

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
                    $.quote6,
                )
            )
        ),

        quote1: $ => gen_quote($, 1),
        quote2: $ => gen_quote($, 2),
        quote3: $ => gen_quote($, 3),
        quote4: $ => gen_quote($, 4),
        quote5: $ => gen_quote($, 5),
        quote6: $ => gen_quote($, 6),

        // generic list
        generic_list: $ =>
        prec.right(0,
            repeat1(
                $._any_list_item_level_1,
            )
        ),

        _any_list_item_level_1: $ => gen_any_list_item($, 1),
        _any_list_item_level_2: $ => gen_any_list_item($, 2),
        _any_list_item_level_3: $ => gen_any_list_item($, 3),
        _any_list_item_level_4: $ => gen_any_list_item($, 4),
        _any_list_item_level_5: $ => gen_any_list_item($, 5),
        _any_list_item_level_6: $ => gen_any_list_item($, 6),

        unordered_list1: $ => gen_generic_list_item($, "unordered", 1),
        unordered_list2: $ => gen_generic_list_item($, "unordered", 2),
        unordered_list3: $ => gen_generic_list_item($, "unordered", 3),
        unordered_list4: $ => gen_generic_list_item($, "unordered", 4),
        unordered_list5: $ => gen_generic_list_item($, "unordered", 5),
        unordered_list6: $ => gen_generic_list_item($, "unordered", 6),

        ordered_list1: $ => gen_generic_list_item($, "ordered", 1),
        ordered_list2: $ => gen_generic_list_item($, "ordered", 2),
        ordered_list3: $ => gen_generic_list_item($, "ordered", 3),
        ordered_list4: $ => gen_generic_list_item($, "ordered", 4),
        ordered_list5: $ => gen_generic_list_item($, "ordered", 5),
        ordered_list6: $ => gen_generic_list_item($, "ordered", 6),

        any_todo_state: $ =>
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

        // --------------------------------------------------

        marker: $ =>
        prec.right(0,
            gen_detached_modifier(
                $,

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
                            $.paragraph,
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

        insertion: $ =>
        prec.right(0,
            gen_detached_modifier(
                $,

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

        single_definition: $ => gen_single_rangeable_detached_modifier($, "definition"),
        multi_definition: $ => gen_multi_rangeable_detached_modifier($, "definition"),

        single_footnote: $ => gen_single_rangeable_detached_modifier($, "footnote"),
        multi_footnote: $ => gen_multi_rangeable_detached_modifier($, "footnote"),

        ranged_tag_content: $ =>
        prec.right(0,
            repeat1(
                choice(
                    choice(
                        $.paragraph_segment,
                        alias($.line_break, "_line_break"),
                        alias($.paragraph_break, "_paragraph_break"),
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

        ranged_verbatim_tag_content: $ =>
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
                ),
            ),
        ),

        ranged_verbatim_tag_end: $ =>
        seq(
            alias(
                $.ranged_verbatim_tag_end_prefix,
                "_prefix",
            ),

            alias(
                token.immediate("end"),
                "_name",
            ),
        ),

        ranged_verbatim_tag: $ =>
        prec.right(0,
            seq(
                alias(
                    $.ranged_verbatim_tag_prefix,
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
                        $.ranged_verbatim_tag_content,
                    ),
                ),

                optional($.ranged_verbatim_tag_end),
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
                        $.paragraph,
                        repeat1(
                            choice(
                                $.detached_modifier,
                                $.definition,
                                $.footnote,
                                $.ranged_tag,
                                $.ranged_verbatim_tag,
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

        // TODO(vhyrro): Perhaps exclude the `""` chars from the `tag_param` node
        tag_param: _ => choice(
            token.immediate(/[^\"]\S*/),
            seq(
                token.immediate('"'),
                token.immediate(/[^\n\r\"\s]+/),
                repeat(
                    seq(
                        token.immediate(/[\t\v ]+/),
                        token.immediate(/[^\n\r\"\s]+/),
                    ),
                ),
                token.immediate('"'),
            ),
        ),


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
            $.ranged_verbatim_tag,
            $.carryover_tag_set,
        ),

        indent_segment: $ => seq(
            $.indent_segment_begin,

            repeat(
                choice(
                    $.paragraph,
                    prec(1, alias($.line_break, "_line_break")),
                    prec(1, alias($.paragraph_break, "_paragraph_break")),
                ),
            ),

            optional(
                $.weak_paragraph_delimiter,
            ),
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
            $.spoiler,
            $.superscript,
            $.subscript,
            $.verbatim,
            $.inline_comment,
            $.inline_math,
            $.variable,
        ),
    }
});

function gen_detached_modifier($, prefix, ...rest) {
    return seq(
        prefix,

        optional(
            field(
                "state",
                $.any_todo_state,
            ),
        ),

        ...rest,
    )
}

function gen_heading($, level) {
    let lower_level_heading = []
    for (let i = 0; i + level < 6; i++) {
        lower_level_heading[i] = $["heading" + (i + 1 + level)]
    }

    return prec.right(0,
        gen_detached_modifier(
            $,

            $["heading" + level + "_prefix"],

            field(
                "title",
                $.paragraph_segment,
            ),

            repeat(prec(1, alias($.line_break, "_line_break"))),

            field(
                "content",

                repeat(
                    choice(
                        $.paragraph,

                        alias($.paragraph_break, "_paragraph_break"),
                        $.detached_modifier,
                        $.definition,
                        $.footnote,
                        $.tag,
                        $.horizontal_line,

                        ...lower_level_heading,
                    )
                )
            ),

            optional(
                $.weak_paragraph_delimiter,
            )
        )
    );
}

function gen_any_list_item($, level) {
    if (level == 6) {
        return choice(
            $["unordered_list" + level],
            $["ordered_list" + level],
        );
    }
    return choice(
        $["unordered_list" + level],
        $["ordered_list" + level],
        $["_any_list_item_level_" + (level + 1)],
    );
}

function gen_generic_list_item($, kind, level) {
    lower_level_list_items = [];
    if (level < 6) {
        lower_level_list_items[0] = $["_any_list_item_level_" + (level + 1)]
    }

    return prec.right(0,
        gen_detached_modifier(
            $,

            $[kind + "_list" + level + "_prefix"],

            field(
                "content",
                choice(
                    $.paragraph,
                    $.indent_segment,
                ),
            ),

            repeat(
                choice(
                    ...lower_level_list_items,
                ),
            ),
        ),
    );
}

function gen_quote($, level) {
    lower_level_quotes = [];
    if (level < 6) {
        for (let i = 0; i + level < 6; i++) {
            lower_level_quotes[i] = $["quote" + (i + 1 + level)]
        }
    }

    return prec.right(0,
        gen_detached_modifier(
            $,

            $["quote" + level + "_prefix"],

            field(
                "content",
                choice(
                    $.paragraph,
                    $.indent_segment,
                ),
            ),

            optional(prec(1, alias($.line_break, "_line_break"))),

            repeat(
                choice(
                    ...lower_level_quotes,
                ),
            ),
        ),
    );
}

function gen_attached_modifier($, kind, verbatim) {
    let content_rule = $._attached_modifier_content;
    let precedence = 3;
    if (verbatim) {
        content_rule = $._verbatim_modifier_content;
        precedence = 4;
    }

    return prec.dynamic(precedence,
        seq(
            alias($[kind + "_open"], "_open"),
            content_rule,
            alias($[kind + "_close"], "_close"),
        ),
    );
}

function gen_single_rangeable_detached_modifier($, kind) {
    return prec.right(
        gen_detached_modifier(
            $,

            $["single_" + kind + "_prefix"],

            field(
                "title",
                $.paragraph_segment,
            ),

            repeat(
                prec(1, choice(
                    alias($.line_break, "_line_break"),
                    alias($.paragraph_break, "_paragraph_break"),
                )),
            ),

            field(
                "content",
                $.paragraph,
            ),
        ),
    );
}

function gen_multi_rangeable_detached_modifier($, kind) {
    // NOTE: there used to be a choice rule with an optional standalone suffix
    // so if problems arise with standalone closing nodes, re-add that
    return gen_detached_modifier(
            $,

            $["multi_" + kind + "_prefix"],

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
                        $.paragraph,
                        prec(1, alias($.line_break, "_line_break")),
                        alias($.paragraph_break, "_paragraph_break"),
                        $.detached_modifier,
                        $.tag,
                    ),
                ),
            ),

            field(
                "end",
                $["multi_" + kind + "_suffix"],
            ),
        );
}
