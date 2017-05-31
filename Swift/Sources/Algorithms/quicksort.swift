import Darwin
import Foundation

func arraySwap<T>(_ array:inout UnsafeMutableBufferPointer<T>, _ index1:Int, _ index2:Int) {
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

func partition<T>(array: inout UnsafeMutableBufferPointer<T>, start first:Int, end last:Int, pivotHint:Int? = nil, compare: (T,T) -> Int) -> Int{
  let pivotIndex = selectPivotIndex(hint:pivotHint, start:first, end:last)
  let pivot = array[pivotIndex]

  arraySwap(&array, first, pivotIndex);

  var i = first+1
  var j = last;
  while i<=j {
    if compare(array[i], pivot) < 0 {
      i+=1
    } else if compare(array[j],pivot) > 0 {
      j-=1
    } else {
      arraySwap(&array, i, j);
      i+=1
      j-=1
    }
  }
  arraySwap(&array, first, j);
  return j;
}

func quicksort<T>(_ array: inout UnsafeMutableBufferPointer<T>, start:Int, end:Int, compare: (T,T) -> Int) {
  if end <= start {
    return
  }

  let pivotIndex = partition(array:&array, start:start, end:end, compare:compare)

  quicksort(&array, start:start, end:pivotIndex-1, compare:compare)
  quicksort(&array, start:pivotIndex+1, end:end, compare:compare)
}

public func quicksort<T>(_ array: inout [T], compare: (T,T) -> Int) {
  print("array.count \(array.count)")
  array.withUnsafeMutableBufferPointer { buffer in
    print("end index: \(buffer.endIndex)")
    quicksort(&buffer, start:buffer.startIndex, end: buffer.endIndex - 1, compare: compare)
  }
}
