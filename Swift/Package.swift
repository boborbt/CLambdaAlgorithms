import PackageDescription

let package = Package(
    name: "LabAlgoritmi",
    targets: [
      Target( name: "Lib", dependencies: []),
      Target( name: "Ex1", dependencies: ["Lib", "ExpLib"]),
      Target( name: "ExpLib", dependencies: [])
    ],
    dependencies: [
      .Package(url: "https://github.com/nvzqz/FileKit", majorVersion: 4)
    ]
)
