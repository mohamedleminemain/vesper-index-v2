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
    std::cerr << "usage: vesper-normalize-dump INPUT\n";
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
  std::cout << "tokens=" << store.normalizer.tokens.size() << "\n";
  std::cout << "emitted=" << store.normalizer.emitted.size() << "\n";
  std::cout << "digest=" << store.normalizer.digest << "\n";
  for (const auto& token : store.normalizer.tokens) {
    std::cout << token.source_id
              << " raw=" << token.raw
              << " normalized=" << token.normalized
              << " count=" << token.count << "\n";
  }
  for (const auto& line : store.normalizer.emitted) {
    std::cout << "emit " << line << "\n";
  }
  return 0;
}
