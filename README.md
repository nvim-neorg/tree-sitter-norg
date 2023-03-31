# NFF TreeSitter Parser 

<img src="https://raw.githubusercontent.com/nvim-neorg/neorg/main/res/neorg.svg" width="70" height="70">

A TreeSitter grammar for [Neorg](https://github.com/nvim-neorg/neorg).

## Available Commands
| Command           | Result																				|
| -                 | -																						|
| `yarn`            | installs needed dependencies (only do if you don't have `tree-sitter` in your path)	|
| `yarn gen`		| `tree-sitter generate && node-gyp build`												|
| `yarn test`		| `tree-sitter test`																	|

- `npm` can be used instead of `yarn`
- When `yarn` is used with no args then it's replaced with `node install`

# Features
- Has support for pretty much the entire specification
- Isn't a massive editor hog

> **Warning** Installation on MacOS is going to break if you are using something other than the main branch and apple GCC.

To fix that, Add the following lines to your Tree-Sitter config

```lua
local parser_configs = require('nvim-treesitter.parsers').get_parser_configs()

parser_configs.norg = {
  install_info = {
    url = 'https://github.com/vhyrro/tree-sitter-norg',
    files = {'src/parser.c', 'src/scanner.cc'},
    branch = 'main'
  }
}
```

and to change your compiler, 

`brew install gcc`
and
`sudo ln -s $(which gcc-12) /usr/local/bin/cc`


# :heart: Contribution
If you know a thing or two about TreeSitter and would like to support us by contributing then please do!
If you have any questions you can ask away in the Github issues or on our discord! The specification can be found in the
`docs/` directory in the [Neorg Repo](https://github.com/nvim-neorg/neorg).
