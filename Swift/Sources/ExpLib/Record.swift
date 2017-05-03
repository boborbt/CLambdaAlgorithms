import Foundation

public struct Record: Equatable {
  var id: Int
  var field1:String
  var field2:Int
  var field3:Double
}

public func parse(_ string:String) -> Record {
  let fields = string.components(separatedBy:",").map { $0.trimmingCharacters(in: .whitespacesAndNewlines) }
  return Record(
    id: Int(fields[0])!,
    field1: fields[1],
    field2: Int(fields[2])!,
    field3: Double(fields[3])!
  )
}

public func ==(lhs:Record, rhs:Record) -> Bool {
  return lhs.id == rhs.id && lhs.field1 == rhs.field1 && lhs.field2 == rhs.field2 && lhs.field3 == rhs.field3
}
