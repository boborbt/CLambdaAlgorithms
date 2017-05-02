import XCTest
@testable import LabAlgoritmi

class TestQuicksort : XCTestCase {
  func testPartitionOnFullArray1() {
    var array = [19, 1, 32, 45, 11]
    let pivot = partition(array:&array, start:0, end:4, pivotHint: 0, lessThan:<)
    XCTAssertEqual(2, pivot)
    XCTAssertEqual([11,1,19,45,32], array)
  }

  func testPartitionOnFullArray2() {
    var array = [19, 1, 32, 45, 11]
    let pivot = partition(array:&array, start:0, end:4, pivotHint: 1, lessThan:<)
    XCTAssertEqual(0, pivot)
    XCTAssertEqual([1,11,32,45,19], array)
  }

  func testPartitionOnFullArray3() {
    var array = [19, 1, 32, 45, 11]
    let pivot = partition(array:&array, start:0, end:4, pivotHint: 3, lessThan:<)
    XCTAssertEqual(4, pivot)
    XCTAssertEqual([19,1,32,11,45], array)
  }


  func testQuickSortOnFullArray() {
    var array = [19, 1, 32, 45,11, 6, 8, 10, 30]
    quicksort(&array, lessThan:<)
    XCTAssertEqual([1, 6, 8, 10, 11, 19, 30, 32, 45], array)
  }

}
