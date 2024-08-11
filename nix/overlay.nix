{self}: final: prev: let
  luaPackage-override = luaself: luaprev: {
    tree-sitter-norg = luaprev.tree-sitter-norg.overrideAttrs (oa: {
      version = "scm-1";
      knownRockspec = "${self}/tree-sitter-norg-scm-1.rockspec";
      src = self;
      propagatedBuildInputs = [
        # nixpkgs still uses luarocks-build-treesitter-parser (not the -cpp one).
        luaself.luarocks-build-treesitter-parser-cpp
      ];
    });
  };
in {
  lua5_1 = prev.lua5_1.override {
    packageOverrides = luaPackage-override;
  };
  lua51Packages = prev.lua51Packages // final.lua5_1.pkgs;
  vimPlugins =
    prev.vimPlugins
    // {
      tree-sitter-norg = final.neovimUtils.buildNeovimPlugin {luaAttr = "tree-sitter-norg";};
    };
}
