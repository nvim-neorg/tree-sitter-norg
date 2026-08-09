package = "tree-sitter-norg"

version = "scm-1"

rockspec_format = "3.0"

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
}

build_dependencies = {
  "luarocks-build-treesitter-parser ~> 6",
}

build = {
  type = "treesitter-parser-c",
  lang = "norg",
  sources = { "src/parser.c", "src/scanner.c" },
}
