import XCTest
import SwiftTreeSitter
import TreeSitterNorg

final class TreeSitterNorgTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_norg())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Norg grammar")
    }
}
