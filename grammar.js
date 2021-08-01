module.exports = grammar({
  	name: 'norg',

	externals: $ => [
    	$.bold
  	],

  	rules: {
    	/*
        "document",
		"quote",
		"heading1_prefix",
		"heading2_prefix",
		"heading3_prefix",
		"heading4_prefix",
		"heading1",
		"heading2",
		"heading3",
		"heading4",
		"unordered_list_prefix",
		"unordered_list",
		"unordered_link_list_prefix",
		"unordered_link_list",
		"marker",
		"todo_item_prefix",
		"todo_item_suffix",
		"todo_item_done_mark",
		"todo_item_pending_mark",
		"todo_item_undone_mark",
		"todo_item_done",
		"todo_item_pending",
		"todo_item_undone",
		"tag",
		"tag_content",
		"tag_name",
		"tag_parameters",
		"tag_end",
		"carryover_tag",
		"drawer",
		"drawer_content",
		"paragraph",
		"_eof",
		"_soft_paragraph_break",
		"leading_whitespace",
		"escape_sequence",
		"words",
		"paragraph_segment",
		"_detached_modifiers", */
  	}
});
