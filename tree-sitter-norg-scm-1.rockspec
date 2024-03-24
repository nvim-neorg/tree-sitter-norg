package = "tree-sitter-norg"

version = "scm-1"

source = {
  url = "git://github.com/nvim-neorg/tree-sitter-norg",
}

description = {
  summary = "tree-sitter parser for norg",
  homepage = "https://github.com/nvim-neorg/tree-sitter-norg",
  license = "MIT"
}

dependencies = {
  "lua >= 5.1",
  "luarocks-build-treesitter-parser >= 1.2.0",
}

build = {
  type = "treesitter-parser",
  lang = "norg",
  sources = { "src/parser.c", "src/scanner.cc" },
}
