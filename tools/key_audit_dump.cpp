#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: vesper-key-audit-dump <script>\n";
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
  std::cout << "active_keys=" << store.active_keys.size() << "\n";
  std::cout << "leases=" << store.key_audit.leases.size() << "\n";
  std::cout << "history=" << store.key_audit.history.size() << "\n";
  std::cout << "digest=" << store.key_audit.digest << "\n";
  for (const auto& lease : store.key_audit.leases) {
    std::cout << "lease " << lease.lease_id
              << " key=" << lease.key_id
              << " scope=" << lease.scope
              << " state=" << (lease.active ? "active" : "released")
              << " issued=" << lease.issued_epoch
              << " released=" << lease.released_epoch << "\n";
  }
  return 0;
}
