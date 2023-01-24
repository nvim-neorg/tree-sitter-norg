{
  "targets": [
    {
      "target_name": "tree_sitter_norg_binding",
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "src"
      ],
      "sources": [
        "bindings/node/binding.cc",
        "src/parser.c",
        "src/scanner.cc",
      ],
      "cflags_c": [
        "-std=c99",
      ]
    }
  ]
}
