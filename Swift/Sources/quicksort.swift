import Darwin
import Foundation

func arraySwap<T>(_ array:inout [T], _ index1:Int, _ index2:Int) {
  if index1 != index2 {
    swap(&array[index1], &array[index2])
  }
}

func selectPivotIndex(hint pivotHint:Int?, start:Int, end:Int) -> Int {
  if pivotHint != nil {
    return pivotHint!
  }

  return start + Int(arc4random_uniform(UInt32(end - start)))
}

func partition<T>(array: inout [T], start:Int, end: Int, pivotHint:Int? = nil, lessThan: (T,T) -> Bool) -> Int {
  let pivotIndex = selectPivotIndex(hint:pivotHint, start:start, end:end)
  let pivot = array[pivotIndex]

  arraySwap(&array, pivotIndex, end)
  var leftBound = start - 1
  var rightBound = start

  while rightBound < end {
    if !lessThan(pivot, array[rightBound]) {
      leftBound += 1
      arraySwap(&array,leftBound,rightBound)
    }
    rightBound += 1
  }

  arraySwap(&array, leftBound+1, end)

  return leftBound+1
}

func quicksort<T>(_ array: inout [T], start:Int, end:Int, lessThan: (T,T) -> Bool) {
  if end <= start {
    return
  }

  let pivotIndex = partition(array:&array, start:start, end:end, lessThan:lessThan)

  quicksort(&array, start:start, end:pivotIndex-1, lessThan:lessThan)
  quicksort(&array, start:pivotIndex+1, end:end, lessThan:lessThan)
}

func quicksort<T>(_ array: inout [T], lessThan: (T,T) -> Bool) {
  quicksort(&array, start:0, end: array.count - 1, lessThan: lessThan)
}
