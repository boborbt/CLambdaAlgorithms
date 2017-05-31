// Mark: Iterators

struct EdgeIterator<V: Hashable,E>: Sequence, IteratorProtocol {
  var adjacents:[(source:V, adjacents:DictionaryIterator<V,E>?)]
  var current:Int
  typealias Element = (V,V,E)

  init( adjacencies:[(source:V, adjacents:DictionaryIterator<V,E>?)]) {
    self.adjacents = adjacencies
    self.current = 0
  }

  mutating func next() -> (V,V,E)? {
    guard current < adjacents.count else { return nil }
    guard let nextItem = adjacents[current].adjacents?.next() else {
      current += 1
      return next()
    }

    let (source, next_) = (adjacents[current].source, nextItem)
    return (source, next_.key, next_.value)
  }
}

struct VertexIterator<V:Hashable>: Sequence, IteratorProtocol {
  typealias Element = V

  let next_:  () -> V?
  init(iterator: @escaping () -> V? ) {
    next_ = iterator
  }

  mutating func next() -> V? {
    return next_()
  }
}

// Graph data structure definition
class DirectedGraph<V:Hashable, E> {
  typealias Vertex = V
  typealias Edge = E

  var adjacencies:[Vertex:[Vertex:Edge]] = [:]

  static func +=( g:inout DirectedGraph, edge:(source:Vertex, dest:Vertex, info:Edge)) {
    if g.adjacencies[edge.source] == nil {
      g.adjacencies[edge.source] = [:]
    }

    if g.adjacencies[edge.dest] == nil {
      g.adjacencies[edge.dest] = [:]
    }

    g.adjacencies[edge.source]![edge.dest] = edge.info
  }

  subscript(source: Vertex, dest:Vertex) -> Edge? {
    return adjacencies[source]?[dest]
  }

  subscript(source: Vertex) -> EdgeIterator<V,E> {
    return EdgeIterator<V,E>(adjacencies:[(source: source, adjacents: adjacencies[source]?.makeIterator())])
  }

  func vertices() -> VertexIterator<Vertex> {
    var iterator = adjacencies.keys.makeIterator()
    return VertexIterator<V>() {
      return iterator.next()
    }
  }

  func edges() -> EdgeIterator<V,E> {
    return EdgeIterator(adjacencies: adjacencies.map { key,value in return (source: key, adjacents: value.makeIterator()) })
  }
}

class UndirectedGraph<V:Hashable, E>: DirectedGraph<V,E> {
  static func +=( g:inout UndirectedGraph<V,E>, edge:(source:Vertex, dest:Vertex, info:Edge)) {
    if g.adjacencies[edge.source] == nil {
      g.adjacencies[edge.source] = [:]
    }

    if g.adjacencies[edge.dest] == nil {
      g.adjacencies[edge.dest] = [:]
    }

    g.adjacencies[edge.source]![edge.dest] = edge.info
    g.adjacencies[edge.dest]![edge.source] = edge.info
  }
}

// Mark: Operators for edge constructors

infix operator -- : AdditionPrecedence
func --<V,E>(_ vertex:V, _ info:E) -> (vertex:V,info:E) {
  return (vertex:vertex, info:info)
}

// Allows one to create a weighed edge using the syntax v1 -- weight --> v2

infix operator -->: AdditionPrecedence
func --><V,E>(_ vw:(vertex:V, info:E), _ vertex: V) -> (source:V, dest:V, info:E) {
    return (source: vw.vertex, dest:vertex, info: vw.info)
}

// Mark: algorithms

// func mst<V:Hashable, E>(graph:UndirectedGraph<V,E>, lessThan: (E,E) -> Bool) -> UndirectedGraph<V,E> {
//   var result = UndirectedGraph<V,E>()
//   var edgeList = Array<(V,V,E)>()
//
// }
