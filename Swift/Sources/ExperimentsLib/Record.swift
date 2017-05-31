import Foundation

public struct Record {
  public var id: Int32
  public var field1:UnsafeMutablePointer<CChar>
  public var field2:Int32
  public var field3:Float

    init(id: Int32, field1:UnsafeMutablePointer<CChar>, field2:Int32, field3:Float) {
        self.id = id
        self.field1 = field1
        self.field2 = field2
        self.field3 = field3
    }
}

public func parse(_ string:UnsafeMutablePointer<CChar>) -> Record {
  // let fields = string.components(separatedBy:",").map { $0.trimmingCharacters(in: .whitespacesAndNewlines) }
  // let fields = ["1", "test", "2", "10.4"]
  //
  var numFields: Int!
  var buf = UnsafeMutablePointer<CChar>.allocate(capacity:200)
  defer { free(buf) }

  var id:Int32 = 0
  var field2:Int32 = 0
  var field3:Float = 0.0
  var result:Record!

  withUnsafeMutablePointer(to:&id) { idPtr in
    withUnsafeMutablePointer(to:&field2) { field2Ptr in
      withUnsafeMutablePointer(to:&field3) { field3Ptr in
           let args: [CVarArg] = [idPtr, buf, field2Ptr, field3Ptr]

           if vsscanf(string, "%d,%[^,],%d,%lf", getVaList(args)) < 3 {
              print("Something went wrong!")
           } else {
              result = Record(
                id:id,
                field1: strdup(buf),
                field2:field2,
                field3:field3
              )
           }
      }
    }
  }

  return result
}

public func ==(lhs:Record, rhs:Record) -> Bool {
  return lhs.id == rhs.id && lhs.field1 == rhs.field1 && lhs.field2 == rhs.field2 && lhs.field3 == rhs.field3
}
