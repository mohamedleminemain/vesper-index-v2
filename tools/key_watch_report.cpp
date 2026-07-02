#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: vesper-key-watch-report <script>\n";
    return 1;
  }
  std::ifstream input(argv[1], std::ios::binary);
  if (!input) {
    std::cerr << "failed to open input\n";
    return 1;
  }
  const std::vector<std::uint8_t> bytes(
      (std::istreambuf_iterator<char>(input)),
      std::istreambuf_iterator<char>());
  std::vector<vesper::Command> commands;
  auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
  if (!parsed) {
    std::cerr << parsed.message << "\n";
    return 1;
  }
  vesper::Store store;
  const auto result = vesper::execute_commands(store, commands, vesper::Profile::all);
  if (!result) {
    std::cerr << result.message << "\n";
    return 1;
  }

  std::cout << "events=" << store.events.size() << "\n";
  std::cout << "active_keys=" << store.active_keys.size() << "\n";
  std::cout << "retired_keys=" << store.retired_keys.size() << "\n";
  std::cout << "leases=" << store.key_audit.leases.size() << "\n";
  std::cout << "watches=" << store.watches.size() << "\n";
  std::cout << "snapshots=" << store.watch_review.snapshots.size() << "\n";
  std::cout << "notes=" << store.watch_review.notes.size() << "\n";
  std::cout << "checkpoints=" << store.checkpoints.entries.size() << "\n";
  std::cout << "policies=" << store.policy.rules.size() << "\n";
  std::cout << "digest=" << store.last_digest << "\n";
  return 0;
}
