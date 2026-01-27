#include "CompareCommand.h"
#include "RunCommand.h"
#include "SnapshotCommand.h"

#include <iostream>
#include <string>
#include <vector>

namespace {

void print_usage() {
  std::cout << "Usage: CoinRenderTests <subcommand> [args]\n"
            << "Subcommands:\n"
            << "  snapshot   Render a scene or spec to PNG.\n"
            << "  compare    Compare two images with tolerances.\n"
            << "  run        Run the full spec-based suite.\n"
            << "  update     Update baselines (alias for: run --update-baselines).\n";
}

} // namespace

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage();
    return 1;
  }

  const std::string cmd = argv[1];
  if (cmd == "--help" || cmd == "-h" || cmd == "help") {
    print_usage();
    return 0;
  }
  if (cmd == "snapshot") {
    return CoinRenderTests::RunSnapshotCommand(argc - 1, argv + 1);
  }
  if (cmd == "compare") {
    return CoinRenderTests::RunCompareCommand(argc - 1, argv + 1);
  }
  if (cmd == "run") {
    return CoinRenderTests::RunRunCommand(argc - 1, argv + 1);
  }
  if (cmd == "update") {
    std::vector<const char*> args;
    args.reserve(static_cast<size_t>(argc) + 1);
    args.push_back("run");
    args.push_back("--update-baselines");
    for (int i = 2; i < argc; ++i) {
      args.push_back(argv[i]);
    }
    return CoinRenderTests::RunRunCommand(static_cast<int>(args.size()), args.data());
  }

  std::cerr << "Unknown subcommand: " << cmd << '\n';
  print_usage();
  return 1;
}
