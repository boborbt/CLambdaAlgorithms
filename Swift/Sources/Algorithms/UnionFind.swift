struct UnionFind<V: Hashable & Equatable> {
  var sets:[V:V] = [:]
  var ranks:[V:Int] = [:]

  mutating func makeSet(_ v:V) {
    sets[v] = v
    ranks[v] = 0
  }

  mutating func union(_ v1:V, _ v2:V) {
    let v1_ = find(v1)
    let v2_ = find(v2)

    guard v1_ != v2_  else { return }

    switch( ranks[v1_]!, ranks[v2_]! ) {
    case let (v1_rank, v2_rank)  where v1_rank < v2_rank:
      sets[v1] = v2_
    case let (v1_rank, v2_rank) where v2_rank < v1_rank:
      sets[v2] = v1_
    default:
      sets[v1] = v2_
      ranks[v2_]! += 1
    }
  }

  mutating func find(_ v:V) -> V {
    guard sets[v] != v else { return v }

    let v_ = find(sets[v]!)
    sets[v] = v_

    return v_
  }
}
