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
    std::cerr << "usage: vesper-cache-dump INPUT\n";
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
  std::cout << "windows=" << store.cache.windows.size() << "\n";
  std::cout << "target=" << store.cache.target_bytes << "\n";
  std::cout << "total=" << store.cache.total_bytes << "\n";
  std::cout << "evictions=" << store.cache.evictions << "\n";
  for (std::size_t i = 0; i < store.cache.windows.size(); ++i) {
    const auto& window = store.cache.windows[i];
    std::cout << "window " << i
              << " page=" << window.page_id
              << " offset=" << window.offset
              << " bytes=" << window.bytes.size()
              << " hits=" << window.hits
              << " pinned=" << (window.pinned ? 1 : 0) << "\n";
  }
  return 0;
}
