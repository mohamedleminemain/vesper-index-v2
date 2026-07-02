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
}

int main(int argc, char** argv) {
  if (argc != 2) return 2;
  const auto bytes = read_bytes(argv[1]);
  std::vector<vesper::Command> commands;
  if (!vesper::parse_script(bytes.data(), bytes.size(), commands)) return 1;
  vesper::Store store;
  if (!vesper::execute_commands(store, commands, vesper::Profile::all)) return 1;
  std::cout << "selected=";
  if (store.checkpoints.selected.has_value()) {
    std::cout << *store.checkpoints.selected;
  } else {
    std::cout << "none";
  }
  std::cout << "\ncount=" << store.checkpoints.entries.size() << "\n";
  std::cout << "digest=" << store.checkpoints.digest << "\n";
  return 0;
}
