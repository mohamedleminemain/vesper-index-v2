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
    std::cerr << "usage: vesper-block-dump INPUT\n";
    return 2;
  }
  const auto bytes = read_bytes(argv[1]);
  std::vector<vesper::Command> commands;
  auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
  if (!parsed) return 1;
  vesper::Store store;
  auto executed = vesper::execute_commands(store, commands, vesper::Profile::all);
  if (!executed) return 1;
  std::cout << "chunks=" << store.blocks.chunks.size() << "\n";
  std::cout << "block_size=" << store.blocks.block_size << "\n";
  std::cout << "packed=" << store.blocks.packed_bytes << "\n";
  std::cout << "unpacked=" << store.blocks.unpacked_bytes << "\n";
  for (const auto& chunk : store.blocks.chunks) {
    std::cout << "page=" << chunk.page_id << " runs=" << chunk.runs.size()
              << " flat=" << chunk.flat.size() << "\n";
  }
  return 0;
}
