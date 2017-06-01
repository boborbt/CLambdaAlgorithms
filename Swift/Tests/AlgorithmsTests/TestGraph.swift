import XCTest
@testable import Algorithms

class TestGraph : XCTestCase {
  func testDashesOperator() {
    let expected = (vertex:"a", info:100)
    let actual = "a" -| 100

    XCTAssertEqual(expected.vertex, actual.vertex)
    XCTAssertEqual(expected.info, actual.info)
  }

  func testArrowOpeartor() {
    let expected = (source:"a", dest:"b", info: 100)
    let actual:(source:String, dest:String, info:Int) = "a" -| 100 |-> "b"

    XCTAssertEqual(expected.source, actual.source)
    XCTAssertEqual(expected.dest, actual.dest)
    XCTAssertEqual(expected.info, actual.info)

  }

  func testAddDirectedEdge() {
    var g = Graph<String, Int>()

    g += "a" -| 100 |-> "b"

    XCTAssertEqual(100, g["a","b"])
    XCTAssertEqual(nil, g["b","a"])
  }

  func testGetAdjacents() {
    var g = Graph<String, Int>()

    g += ("a" -| 100 |-> "b")
    g += "a" -| 80 |-> "c"
    g += "b" -| 50 |-> "c"
    g += "d" -| 21 |-> "d"

    var count = 0
    for (_,_,_) in g["a"] {
      count += 1
    }

    XCTAssertEqual(2, count)
  }

  func testVerticesIterator() {
    var g = Graph<String, Int>()

    g += "a" -| 100 |-> "b"
    g += "a" -| 80 |-> "c"
    g += "b" -| 50 |-> "c"
    g += "d" -| 21 |-> "d"

    let vertices = Array<String>(g.vertices()).sorted()
    XCTAssertEqual(["a","b","c","d"], vertices)
  }

  func testAllEdgesGetter() {
    var g = Graph<String, Int>()

    g += "a" -| 100 |-> "b"
    g += "a" -| 80 |-> "c"
    g += "b" -| 50 |-> "c"
    g += "d" -| 21 |-> "d"

    let edges = Array<(String, String, Int)>(g.edges()).sorted(by: { v1, v2 in
      return v1.0 < v2.0 || (v1.0 == v2.0 && v1.1 < v2.1) || (v1.0 == v2.0 && v1.1 == v2.1 && v1.2 < v2.2)
    })

    XCTAssertTrue( [("a","b",100), ("a", "c", 80), ("b", "c", 50), ("d", "d", 21)].elementsEqual(edges, by:==) )
  }
}
