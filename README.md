# Norg Treesitter Parser

<img src="https://raw.githubusercontent.com/nvim-neorg/neorg/main/res/neorg.svg" width="70" height="70">

A tree-sitter grammar for [Neorg](https://github.com/nvim-neorg/neorg)'s file format, Norg.

## Available Commands
| Command           | Result																				|
| -                 | -																						|
| `tree-sitter generate`		| Generates the parser files necessary for building
| `tree-sitter test`		| Runs a comprehensive tests suite with known edge cases
| `tree-sitter build`		| Builds a `norg.so` file that can be used by your favourite editor |

# Features
- Has support for the entire Norg V1 specification
- Isn't a massive editor hog

# Context

This version of the tree-sitter parser is a full implementation of the Norg V1 specification.
Unfortunately, this parser has a lot of historical cruft and overly relies on its `scanner.c`.

In the future, we will be working and releasing a revised version of the format, Norg V2,
which will be even simpler to implement in a parser form. V2 will behave identically for almost all files,
but the most niche features of the language will be ironed out, so don't worry about breakages related to your notes.

# :heart: Contribution
If you know a thing or two about tree-sitter and would like to support us by contributing then please do!
If you have any questions you can ask away in the Github issues or on our discord! The specification can be found in the
`docs/` directory in the [Neorg Repo](https://github.com/nvim-neorg/neorg).
