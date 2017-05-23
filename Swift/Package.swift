import PackageDescription

let package = Package(
    name: "SwiftSorting",
    targets: [
      Target( name: "Lib", dependencies: []),
      Target( name: "Ex1", dependencies: ["Lib", "ExpLib"]),
      Target( name: "ExpLib", dependencies: [])
    ],
    dependencies: []
)
