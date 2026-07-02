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
    std::cerr << "usage: vesper-shard-dump INPUT\n";
    return 2;
  }
  const auto bytes = read_bytes(argv[1]);
  std::vector<vesper::Command> commands;
  auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
  if (!parsed) {
    std::cerr << parsed.message << "\n";
    return 1;
  }
  vesper::Store store;
  auto executed = vesper::execute_commands(store, commands, vesper::Profile::all);
  if (!executed) {
    std::cerr << executed.message << "\n";
    return 1;
  }
  std::cout << "count=" << store.shards.count << "\n";
  std::cout << "placements=" << store.shards.placements.size() << "\n";
  std::cout << "digest=" << store.shards.digest << "\n";
  for (std::size_t i = 0; i < store.shards.load.size(); ++i) {
    std::cout << "load[" << i << "]=" << store.shards.load[i] << "\n";
  }
  for (const auto& entry : store.shards.placements) {
    std::cout << entry.kind
              << " shard=" << entry.shard
              << " id=" << entry.object_id
              << " weight=" << entry.weight << "\n";
  }
  return 0;
}
