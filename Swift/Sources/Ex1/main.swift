import Lib
import ExpLib
import Darwin
import CoreFoundation

var lineCap:Int = 1024
var line:UnsafeMutablePointer<CChar>? = UnsafeMutablePointer<CChar>.allocate(capacity: lineCap)

defer { free(line) }

func lineGenerator(file:UnsafeMutablePointer<FILE>) -> AnyIterator<UnsafeMutablePointer<CChar>>
{
  return AnyIterator { () -> UnsafeMutablePointer<CChar>? in
    return getline(&line, &lineCap, file) > 0 ? line : nil
  }
}

if CommandLine.arguments.count != 2 {
  print("Usage: Ex1 <filename>")
  exit(1)
}

let fileName = CommandLine.arguments[1]

let file = fopen(fileName, "r")
let fileLines = lineGenerator(file:file!)
var count = 0
var array:[Record] = []

print(MemoryLayout<Record>.stride)

for line in fileLines {
  if count % 1000000 == 0 {
    print(count)
  }

  array.append(parse(line))

  count += 1
}

print("Sorting...")

let startTime = CFAbsoluteTimeGetCurrent()
quicksort(&array, compare: { (r1:Record, r2:Record) -> Int in
  return Int(r1.field2 - r2.field2)
})
// array.sort(by:{ (r1:Record, r2:Record) -> Bool in
//   return r1.field2 < r2.field2
// })
let endTime = CFAbsoluteTimeGetCurrent()

print("Elapsed: \(endTime - startTime)s")

for record in array[0...9] {
    print(record)
}
