// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "TreeSitterNorg",
    products: [
        .library(name: "TreeSitterNorg", targets: ["TreeSitterNorg"]),
    ],
    dependencies: [],
    targets: [
        .target(name: "TreeSitterNorg",
                path: ".",
                exclude: [
                    "binding.gyp",
                    "Cargo.toml",
                    "default.nix",
                    "flake.nix",
                    "flake.lock",
                    "grammar.js",
                    "LICENSE",
                    "package.json",
                    "README.md",
                    "src/grammar.json",
                    "src/node-types.json",
                    "test",
                ],
                sources: [
                    "src/parser.c",
                    "src/scanner.cc"
                ],
                publicHeadersPath: "bindings/swift",
                cSettings: [.headerSearchPath("src")]),
    ],
    cxxLanguageStandard: .cxx14
)
