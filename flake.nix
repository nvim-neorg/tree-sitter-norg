{
  description = "tree-sitter parser for Neorg";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = inputs @ {
    self,
    nixpkgs,
    flake-parts,
    ...
  }:
    flake-parts.lib.mkFlake {inherit inputs;} {
      systems = [
        "x86_64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      perSystem = {system, ...}: let
        pkgs = import nixpkgs {
          inherit system;
          overlays = [
            self.overlays.default
          ];
        };
      in {
        devShells.default = pkgs.mkShell {
          name = "tree-sitter-norg devShell";
          buildInputs = with pkgs;
          with pkgs; [
            nil
            alejandra
            clang
            tree-sitter
          ];
        };
        packages = rec {
          default = tree-sitter-norg;
          inherit (pkgs.lua51Packages) tree-sitter-norg;
        };
      };
      flake = {
        overlays.default = import ./nix/overlay.nix {inherit self;};
      };
    };
}
