import PackageDescription

let package = Package(
    name: "SwiftSorting",
    targets: [
      Target( name: "Algorithms", dependencies: []),
      Target( name: "Experiments", dependencies: ["Algorithms", "ExperimentsLib"]),
      Target( name: "ExperimentsLib", dependencies: [])
    ],
    dependencies: []
)
