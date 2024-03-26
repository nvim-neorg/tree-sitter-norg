{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    flake-compat = {
      url = "github:edolstra/flake-compat";
      flake = false;
    };
  };
  outputs = { self, nixpkgs, flake-utils, ... }@inputs:
    (flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        grammar = pkgs.callPackage
          "${nixpkgs}/pkgs/development/tools/parsing/tree-sitter/grammar.nix" { };
      in rec {
        packages.tree-sitter-norg = grammar {
          language = "norg";
          source = ./.;
          inherit (pkgs.tree-sitter) version;
        };
        defaultPackage = packages.tree-sitter-norg;
        devShell = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            python3
            nodejs
            nodePackages.node-gyp
            tree-sitter
          ];
        };
      }));
}
