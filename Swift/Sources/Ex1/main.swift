import Lib
import ExpLib
import FileKit
import Darwin

var lineCap:Int = 1024
var line:UnsafeMutablePointer<CChar>? = UnsafeMutablePointer<CChar>.allocate(capacity: lineCap)

func lineGenerator(file:UnsafeMutablePointer<FILE>) -> AnyIterator<String>
{
  return AnyIterator { () -> String? in
    return getline(&line, &lineCap, file) > 0 ? String(cString:line!) : nil
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

for line in fileLines {
  if count % 100000 == 0 {
    print(count)
  }

  array.append(parse(line))

  count += 1

  if count % 500000 == 0 {
    break
  }

}

free(line)

// let file = TextFile(path: Path(fileName))
// let reader = file.streamReader("\n")!
//
// var count = 0
// var array:[Record] = []
//
// for line in reader {
//   if count % 100000 == 0 {
//     print(count)
//   }
//   array.append(parse(line))
//   count += 1
//
//   if count % 500000 == 0 {
//     break
//   }
// }
