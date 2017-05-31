import XCTest
@testable import Algorithms

class TestUnionFind : XCTestCase {
  func testMakeSet() {
    var uf = UnionFind<Int>()

    uf.makeSet(4)
    uf.makeSet(2)

    XCTAssertEqual(4, uf.sets[4])
    XCTAssertEqual(2, uf.sets[2])
    XCTAssertEqual(0, uf.ranks[4])
    XCTAssertEqual(0, uf.ranks[2])
  }

  func testFind() {
    var uf = UnionFind<Int>()

    uf.makeSet(1)
    uf.makeSet(2)
    uf.makeSet(3)
    uf.makeSet(4)

    uf.sets[2] = 1
    uf.sets[3] = 2
    uf.sets[4] = 3

    XCTAssertEqual(1, uf.find(4))
  }

  func testUnion() {
    var uf = UnionFind<Int>()

    uf.makeSet(1)
    uf.makeSet(2)
    uf.makeSet(3)
    uf.makeSet(4)

    uf.union(1,2)

    XCTAssertEqual(2, uf.sets[1]!)
    XCTAssertEqual(1, uf.ranks[2]!)

    uf.union(3, 1)

    XCTAssertEqual(2, uf.sets[3]!)
    XCTAssertEqual(1, uf.ranks[2]!)
  }
}
