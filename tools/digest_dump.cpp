#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

namespace {
std::vector<std::uint8_t> read_bytes(const char* path) {
  std::ifstream input(path, std::ios::binary);
  return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), {});
}
}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: vesper-digest-dump INPUT\n";
    return 2;
  }
  const auto bytes = read_bytes(argv[1]);
  std::vector<vesper::Command> commands;
  auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
  if (!parsed) return 1;
  vesper::Store store;
  auto executed = vesper::execute_commands(store, commands, vesper::Profile::all);
  if (!executed) return 1;
  std::cout << "entries=" << store.digests.entries.size() << "\n";
  std::cout << "verified=" << store.digests.verified << "\n";
  std::cout << "matched=" << (store.digests.matched ? 1 : 0) << "\n";
  for (const auto& entry : store.digests.entries) {
    std::cout << entry.kind << " id=" << entry.object_id
              << " digest=" << entry.digest << "\n";
  }
  return 0;
}
