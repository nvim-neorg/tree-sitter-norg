module.exports = grammar({
    name: 'norg',

    inline: $ => [
        $.attached_modifier,
        $.heading,
        $.nestable_detached_modifier,
        $.rangeable_detached_modifier,
        $.tag,

        $._indent_segment_contents1,
        $._indent_segment_contents2,
        $._indent_segment_contents3,
        $._indent_segment_contents4,
        $._indent_segment_contents5,
        $._indent_segment_contents6,

        $._any_list_item_level_1,
        $._any_list_item_level_2,
        $._any_list_item_level_3,
        $._any_list_item_level_4,
        $._any_list_item_level_5,
        $._any_list_item_level_6,
    ],

    conflicts: $ => [
        // Conflict arises when parsing unclosed markup
        // Input could be:
        //  *text*
        // or
        //  *text
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
        [$.inline_macro, $._conflict_open],

        [$._paragraph_element],

        [$.quote1],
        [$.quote2],
        [$.quote3],
        [$.quote4],
        [$.quote5],
        [$.quote6],

        [$.ordered_list1],
        [$.ordered_list2],
        [$.ordered_list3],
        [$.ordered_list4],
        [$.ordered_list5],
        [$.ordered_list6],

        [$.unordered_list1],
        [$.unordered_list2],
        [$.unordered_list3],
        [$.unordered_list4],
        [$.unordered_list5],
        [$.unordered_list6],
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

        $.detached_modifier_extension_begin,
        $.mod_extension_delimiter,
        $.detached_modifier_extension_end,

        $._priority,
        $._timestamp,
        $.todo_item_undone,
        $.todo_item_pending,
        $.todo_item_done,
        $.todo_item_on_hold,
        $.todo_item_cancelled,
        $.todo_item_urgent,
        $.todo_item_uncertain,
        $._todo_item_recurring,

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

        $.single_definition_prefix,
        $.multi_definition_prefix,
        $.multi_definition_suffix,

        $.single_footnote_prefix,
        $.multi_footnote_prefix,
        $.multi_footnote_suffix,

        $.single_table_cell_prefix,
        $.multi_table_cell_prefix,
        $.multi_table_cell_suffix,

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
        $.link_target_line_number,
        $.link_target_wiki,
        $.link_target_generic,
        $.link_target_external_file,
        $.link_target_timestamp,
        $.link_target_definition,
        $.link_target_footnote,
        $.link_target_heading1,
        $.link_target_heading2,
        $.link_target_heading3,
        $.link_target_heading4,
        $.link_target_heading5,
        $.link_target_heading6,

        $.timestamp_data,
        $.priority_data,

        $.tag_delimiter,

        $.macro_tag_prefix,
        $.macro_tag_end_prefix,
        $.ranged_tag_prefix,
        $.ranged_tag_end_prefix,
        $.ranged_verbatim_tag_prefix,
        $.ranged_verbatim_tag_end_prefix,

        $.infirm_tag_prefix,
        $.weak_carryover_prefix,
        $.strong_carryover_prefix,

        $.link_modifier,
        $.intersecting_modifier,

        $.attached_mod_extension_begin,
        $.attached_mod_extension_end,

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

        $.inline_macro_open,
        $.inline_macro_close,

        $.free_form_open,
        $.free_form_close,

        $.inline_link_target_open,
        $.inline_link_target_close,

        $.slide_begin,
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
                        $.nestable_detached_modifier,
                        $.rangeable_detached_modifier,
                        $.table,
                        $.tag,
                        $.horizontal_line,
                        $.strong_paragraph_delimiter,
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
        // objects and line breaks.
        paragraph: $ =>
        prec.right(0,
            seq(
                optional($.strong_carryover_set),
                repeat1(
                    choice(
                        $.paragraph_segment,
                        $.intersecting_modifier,
                        alias($.line_break, "_line_break"),
                    ),
                ),
            ),
        ),

        _non_infectable_paragraph: $ =>
        prec.right(0,
            repeat1(
                choice(
                    alias($._non_infectable_paragraph_segment, $.paragraph_segment),
                    alias($.line_break, "_line_break"),
                ),
            ),
        ),

        // A paragraph segment can contain any paragraph element.
        paragraph_segment: $ =>
        prec.right(0,
            seq(
                optional($.weak_carryover_set),
                repeat1(
                    choice(
                        $._paragraph_element,
                        alias($._conflict_open, "_word"),
                    ),
                ),
            ),
        ),

        _non_infectable_paragraph_segment: $ =>
        prec.right(0,
            repeat1(
                choice(
                    $._paragraph_element,
                    alias($._conflict_open, "_word"),
                ),
            ),
        ),

        // The attached modifiers cannot contain a `paragraph_segment` directly,
        // because they:
        //   - require a higher precedence of their internals
        //   - allow line breaks within themselves
        _attached_modifier_content: $ =>
        prec.right(1,
            repeat1(
                choice(
                    $._paragraph_element,
                    alias($.line_break, "_line_break"),
                    alias($._conflict_open, "_word"),
                ),
            ),
        ),

        // Same as the non-verbatim modifier contents but using verbatim
        // elements instead. It cannot contain `_conflict_open` because all of
        // them are aliased to become verbatim.
        _verbatim_modifier_content: $ =>
        prec.right(1,
            repeat1(
                choice(
                    $._verbatim_paragraph_element,
                    alias($.line_break, "_line_break"),
                    $.escape_sequence,
                ),
            ),
        ),

        _free_form_verbatim_modifier_content: $ =>
        prec.right(1,
            repeat1(
                choice(
                    $._verbatim_paragraph_element,
                    alias($.line_break, "_line_break"),
                    prec(1, alias($.escape_sequence_prefix, "_word")),
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
        // The escape sequence is handled outside of this in order to
        // distinguish standard and free-form verbatim markup.
        _verbatim_paragraph_element: $ =>
        choice(
            alias($.word, "_word"),
            alias($.space, "_space"),
            alias($.trailing_modifier, "_word"),
            alias($.link_modifier, "_word"),
            prec.dynamic(5, alias($._conflict_open, "_word")),
            prec.dynamic(5, alias($._conflict_close, "_word")),
            alias($.inline_link_target_open, "_word"),
            prec(1, alias($.inline_link_target_close, "_word")),
            alias($.link_description_begin, "_word"),
            prec(1, alias($.link_description_end, "_word")),
            alias($.attached_mod_extension_begin, "_word"),
            prec(1, alias($.attached_mod_extension_end, "_word")),
            alias($.link_location_begin, "_word"),
            prec(1, alias($.link_location_end, "_word")),
            alias($.link_file_begin, "_word"),
            alias($.link_file_end, "_word"),
            alias($.link_file_text, "_word"),
            alias($.link_target_url, "_word"),
            alias($.link_target_line_number, "_word"),
            alias($.link_target_wiki, "_word"),
            alias($.link_target_generic, "_word"),
            alias($.link_target_external_file, "_word"),
            alias($.link_target_timestamp, "_word"),
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
        inline_macro: $ => gen_attached_modifier($, "inline_macro", true),

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
            alias($.inline_macro_open, "_word"),

            alias($.free_form_open, "_word"),
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
            alias($.inline_macro_close, "_word"),

            alias($.free_form_close, "_word"),

            alias($.attached_mod_extension_end, "_word"),
            alias($.link_location_end, "_word"),
            alias($.link_description_end, "_word"),
            alias($.inline_link_target_close, "_word"),

            alias($.mod_extension_delimiter, "_word"),
            alias($.detached_modifier_extension_end, "_word"),
        ),

        // A backslash followed by the escape token (e.g. \*)
        escape_sequence: $ =>
        seq(
            $.escape_sequence_prefix,

            field(
                "token",
                // TODO: why does this work but [.\n] does not?!
                alias(token.immediate(/.|\n/), $.any_char),
            ),
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
            prec.right(1,
                seq(
                    $._link_target_markup,
                    repeat(
                        seq(
                            alias($.intersecting_modifier, $.link_target_scope),
                            $._link_target_markup,
                        ),
                    ),
                ),
            ),
            $._link_target_verbatim,
        ),

        _link_target_markup: $ =>
        seq(
            field("type",
                choice(
                    $.link_target_wiki,
                    $.link_target_line_number,
                    $.link_target_generic,
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
                    $.link_target_timestamp,
                ),
            ),
            field("text", alias($._link_verbatim_content, $.paragraph)),
        ),

        link: $ =>
        prec.right(2,
            seq(
                $.link_location,
                optional(
                    $.link_description,
                ),
                optional(
                    $.attribute,
                ),
            ),
        ),

        anchor_declaration: $ =>
        prec.right(1,
            seq(
                $.link_description,
                optional(
                    $.link_description,
                ),
                optional(
                    $.attribute,
                ),
            ),
        ),

        anchor_definition: $ =>
        prec(2,
            seq(
                $.link_description,
                $.link_location,
                optional(
                    $.attribute,
                ),
            ),
        ),

        // Attached modifier extension: attributes
        attribute: $ => seq(
            alias($.attached_mod_extension_begin, "_begin"),
            field("value", $.word),
            alias($.attached_mod_extension_end, "_end"),
        ),

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
        prec.right(
            seq(
                optional($.strong_carryover_set),
                repeat1(
                    choice(
                        $.quote1,
                        $.quote2,
                        $.quote3,
                        $.quote4,
                        $.quote5,
                        $.quote6,
                    ),
                ),
            ),
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
            seq(
                optional($.strong_carryover_set),
                repeat1(
                    $._any_list_item_level_1,
                ),
            ),
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

        priority: $ => seq(
            $._priority,
            field("level", $.priority_data),
        ),

        timestamp: $ => seq(
            $._timestamp,
            field("timestamp", $.timestamp_data),
        ),

        todo_item_recurring: $ => seq(
            $._todo_item_recurring,
            optional(
                field(
                    "recurrence",
                    $.timestamp_data,
                ),
            ),
        ),

        detached_modifier_extension: $ =>
        seq(
            alias($.detached_modifier_extension_begin, "_begin"),
            choice(
                $.priority,
                $.timestamp,
                $.todo_item_undone,
                $.todo_item_pending,
                $.todo_item_done,
                $.todo_item_on_hold,
                $.todo_item_cancelled,
                $.todo_item_urgent,
                $.todo_item_uncertain,
                $.todo_item_recurring,
            ),
            repeat(
                seq(
                    alias($.mod_extension_delimiter, "_delimiter"),
                    choice(
                        $.priority,
                        $.timestamp,
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
            ),
            alias($.detached_modifier_extension_end, "_end"),
        ),

        // --------------------------------------------------

        single_definition: $ => gen_single_rangeable_detached_modifier($, "definition"),
        multi_definition: $ => gen_multi_rangeable_detached_modifier($, "definition"),

        single_footnote: $ => gen_single_rangeable_detached_modifier($, "footnote"),
        multi_footnote: $ => gen_multi_rangeable_detached_modifier($, "footnote"),

        single_table_cell: $ => gen_single_rangeable_detached_modifier($, "table_cell"),
        multi_table_cell: $ => gen_multi_rangeable_detached_modifier($, "table_cell"),

        // A table
        table: $ =>
        prec.right(
            seq(
                optional($.strong_carryover_set),
                repeat1(
                    choice(
                        $.single_table_cell,
                        $.multi_table_cell,
                    ),
                ),
            ),
        ),

        definition_list: $ =>
        prec.right(
            seq(
                optional($.strong_carryover_set),
                repeat1(
                    choice(
                        $.single_definition,
                        $.multi_definition,
                    ),
                ),
            ),
        ),

        footnote_list: $ =>
        prec.right(
            seq(
                optional($.strong_carryover_set),
                repeat1(
                    choice(
                        $.single_footnote,
                        $.multi_footnote,
                    ),
                ),
            ),
        ),

        macro_tag_content: $ =>
        prec.right(0,
            repeat1(
                choice(
                    choice(
                        $.paragraph,
                        alias($.paragraph_break, "_paragraph_break"),
                    ),

                    field(
                        "nested_tag",
                        $.ranged_tag
                    ),
                ),
            ),
        ),

        macro_tag_end: $ => gen_ranged_tag_end($, "macro"),

        macro_tag: $ => gen_ranged_tag($, "macro_tag"),

        ranged_tag_content: $ =>
        prec.right(0,
            repeat1(
                choice(
                    choice(
                        $.paragraph,
                        alias($.paragraph_break, "_paragraph_break"),
                    ),

                    field(
                        "nested_tag",
                        $.ranged_tag
                    ),
                ),
            ),
        ),

        ranged_tag_end: $ => gen_ranged_tag_end($, "ranged"),

        ranged_tag: $ => gen_ranged_tag($, "ranged_tag"),

        ranged_verbatim_tag_content: $ =>
        prec.right(0,
            repeat1(
                choice(
                    alias(
                        choice(
                            $.paragraph,
                            alias($.paragraph_break, "_paragraph_break"),
                        ),
                        "_segment",
                    ),
                ),
            ),
        ),

        ranged_verbatim_tag_end: $ => gen_ranged_tag_end($, "ranged_verbatim"),

        ranged_verbatim_tag: $ => gen_ranged_tag($, "ranged_verbatim_tag"),

        weak_carryover_set: $ =>
        repeat1(
            $.weak_carryover,
        ),

        weak_carryover: $ => gen_single_tag($, "weak_carryover"),

        strong_carryover_set: $ =>
        repeat1(
            $.strong_carryover,
        ),

        strong_carryover: $ => gen_single_tag($, "strong_carryover"),

        infirm_tag: $ => gen_single_tag($, "infirm_tag"),

        tag_name: $ =>
        seq(
            $.word,

            repeat(
                seq(
                    alias(
                        $.tag_delimiter,
                        "_delimiter",
                    ),

                    $.word,
                ),
            ),
        ),

        tag_param: $ =>
        choice(
            alias($.word, "_lowercase"),
            // TODO:
            // or backslash
            // or variable
        ),

        tag_parameters: $ =>
        seq(
            $.tag_param,

            repeat(
                seq(
                    alias($.space, "_space"),
                    optional($.tag_param),
                ),
            ),
        ),

        tag: $ =>
        choice(
            $.infirm_tag,
            $.macro_tag,
            $.ranged_tag,
            $.ranged_verbatim_tag,
        ),

        slide: $ => seq(
            alias($.slide_begin, "_slide"),
            choice(
                $.tag,
                $.rangeable_detached_modifier,
                // TODO: Add nestable detached modifiers
                // Currently they cause the parser to become absolutely massive.
            ),
        ),

        indent_segment1: $ => gen_indent_segment($, 1),
        indent_segment2: $ => gen_indent_segment($, 2),
        indent_segment3: $ => gen_indent_segment($, 3),
        indent_segment4: $ => gen_indent_segment($, 4),
        indent_segment5: $ => gen_indent_segment($, 5),
        indent_segment6: $ => gen_indent_segment($, 6),

        _indent_segment_contents1: $ => gen_indent_segment_contents($, 1),
        _indent_segment_contents2: $ => gen_indent_segment_contents($, 2),
        _indent_segment_contents3: $ => gen_indent_segment_contents($, 3),
        _indent_segment_contents4: $ => gen_indent_segment_contents($, 4),
        _indent_segment_contents5: $ => gen_indent_segment_contents($, 5),
        _indent_segment_contents6: $ => gen_indent_segment_contents($, 6),

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

        // A list of detached modifiers (excluding headings and definitons)
        nestable_detached_modifier: $ =>
        choice(
            $.quote,
            $.generic_list,
        ),

        rangeable_detached_modifier: $ =>
        choice(
            $.definition_list,
            $.footnote_list,
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
            $.inline_macro,
        ),
    }
});

function gen_single_tag($, kind) {
    return seq(
        alias(
            $[kind + "_prefix"],
            "_prefix",
        ),

        field(
            "name",
            $.tag_name,
        ),

        optional(alias($.space, "_space")),

        optional($.tag_parameters),

        alias($.line_break, "_line_break"),
    );
}

function gen_ranged_tag($, kind) {
    return prec.right(0,
        seq(
            optional($.strong_carryover_set),

            optional($.weak_carryover_set),

            gen_single_tag($, kind),

            field(
                "content",
                optional(
                    $[kind + "_content"],
                ),
            ),

            $[kind + "_end"],
        ),
    );
}

function gen_ranged_tag_end($, kind) {
    return seq(
        alias(
            $[kind + "_tag_end_prefix"],
            "_prefix",
        ),

        alias(
            token.immediate("end"),
            "_name",
        ),
    );
}

function gen_detached_modifier($, prefix, ...rest) {
    return seq(
        optional($.weak_carryover_set),

        prefix,

        optional(
            field(
                "state",
                $.detached_modifier_extension,
            ),
        ),

        ...rest,
    );
}

function gen_heading($, level) {
    let lower_level_heading = []
    for (let i = 0; i + level < 6; i++) {
        lower_level_heading[i] = $["heading" + (i + 1 + level)]
    }

    return prec.right(0,
        seq(
            optional($.strong_carryover_set),

            gen_detached_modifier(
                $,

                $["heading" + level + "_prefix"],

                indent_segment_or($, 1,
                    field(
                        "title",
                        $.paragraph_segment,
                    ),

                    repeat(prec(1, alias($.line_break, "_line_break"))),
                ),

                field(
                    "content",

                    repeat(
                        choice(
                            $.paragraph,

                            alias($.paragraph_break, "_paragraph_break"),
                            $.nestable_detached_modifier,
                            $.rangeable_detached_modifier,
                            $.table,
                            $.tag,
                            $.horizontal_line,

                            ...lower_level_heading,
                        )
                    )
                ),

                optional(
                    $.weak_paragraph_delimiter,
                ),
            ),
        ),
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

    return gen_detached_modifier(
        $,

        $[kind + "_list" + level + "_prefix"],

        indent_segment_or($, level, field(
            "content",
            alias($._non_infectable_paragraph, $.paragraph),
        )),

        repeat(
            choice(
                ...lower_level_list_items,
            ),
        ),
    );
}

function gen_quote($, level) {
    lower_level_quotes = [];

    for (let i = 0; i + level < 6; i++) {
        lower_level_quotes[i] = $["quote" + (i + 1 + level)]
    }

    return gen_detached_modifier(
        $,

        $["quote" + level + "_prefix"],

        indent_segment_or($, level, field(
            "content",
            alias($._non_infectable_paragraph, $.paragraph),
        )),

        repeat(
            choice(
                ...lower_level_quotes,
            ),
        ),
    );
}

function gen_attached_modifier($, kind, verbatim) {
    let precedence = 3;
    let content = $._attached_modifier_content;
    let free_form_content = $._attached_modifier_content;

    if (verbatim) {
        content = $._verbatim_modifier_content;
        free_form_content = $._free_form_verbatim_modifier_content;
        precedence = 5;
    }

    return prec.dynamic(precedence,
        seq(
            alias($[kind + "_open"], "_open"),
            choice(
                prec(1, seq(
                    $.free_form_open,
                    free_form_content,
                    $.free_form_close,
                )),
                content,
            ),
            alias($[kind + "_close"], "_close"),
            optional($.attribute),
        ),
    );
}

function gen_single_rangeable_detached_modifier($, kind) {
    return gen_detached_modifier(
        $,

        $["single_" + kind + "_prefix"],

        indent_segment_or($, 1, field(
            "title",
            $.paragraph_segment,
        )),

        repeat(
            prec(1, choice(
                alias($.intersecting_modifier, "_intersecting_modifier"),
                alias($.line_break, "_line_break"),
                alias($.paragraph_break, "_paragraph_break"),
            )),
        ),

        field(
            "content",
            $.paragraph,
        ),
    );
}

function gen_multi_rangeable_detached_modifier($, kind, include_strong) {
    return gen_detached_modifier(
        $,

        $["multi_" + kind + "_prefix"],

        indent_segment_or($, 1, field(
            "title",
            $.paragraph_segment,
        )),

        choice(
            alias($.intersecting_modifier, "_intersecting_modifier"),
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
                    $.rangeable_detached_modifier,
                    $.table,
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

function gen_indent_segment_contents($, level) {
    const numbered_items = level < 6 ? [
        "unordered_list",
        "ordered_list",
        "quote",
        "_indent_segment_contents",
    ] : [];

    if (level < 6) {
        return prec(1,
            choice(
                $.rangeable_detached_modifier,
                $.table,
                $.tag,
                ...numbered_items.map(item => $[item + (level + 1)]),
            ),
        );
    } else {
        return prec(1,
            choice(
                $.rangeable_detached_modifier,
                $.table,
                $.tag
            ),
        );
    }
}

function gen_indent_segment($, level) {
    return prec.right(seq(
        $.indent_segment_begin,

        repeat(
            prec(1,
                prec.dynamic(2, choice(
                    $.paragraph,
                    alias($.line_break, "_line_break"),
                    alias($.paragraph_break, "_paragraph_break"),
                    $["_indent_segment_contents" + level],
                )),
            ),
        ),

        optional(
            // Higher dynamic precedence here is required as clashes
            // can occur with headings having paragraph delimiters.
            prec.dynamic(2, $.weak_paragraph_delimiter),
        ),
    ));
}

function indent_segment_or($, level, ...other) {
    return choice(
        $["indent_segment" + (level ? level : 1)],
        $.slide,
        seq(...other),
    );
}
