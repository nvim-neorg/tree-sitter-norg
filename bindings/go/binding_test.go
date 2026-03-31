package tree_sitter_norg_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_norg "github.com/nvim-neorg/tree-sitter-norg/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_norg.Language())
	if language == nil {
		t.Errorf("Error loading Norg grammar")
	}
}
