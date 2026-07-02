#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

std::vector<std::uint8_t> read_bytes(const char* path) {
  std::ifstream input(path, std::ios::binary);
  return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), {});
}

std::string read_text(const char* path) {
  std::ifstream input(path, std::ios::binary);
  return std::string(std::istreambuf_iterator<char>(input), {});
}

}  // namespace

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: vesper-seed-merge OUTPUT INPUT...\n";
    return 2;
  }

  std::ofstream output(argv[1], std::ios::binary);
  if (!output) {
    std::cerr << "cannot open " << argv[1] << "\n";
    return 1;
  }

  std::uint64_t digest = 0;
  output << "VSP1\n";
  for (int i = 2; i < argc; ++i) {
    const auto bytes = read_bytes(argv[i]);
    std::vector<vesper::Command> commands;
    const auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
    if (!parsed) {
      std::cerr << "skip invalid " << argv[i] << ": " << parsed.message << "\n";
      continue;
    }
    for (const auto& command : commands) {
      output << command.verb;
      for (const auto& arg : command.args) output << ' ' << arg;
      output << "\n";
      digest ^= vesper::hash_bytes(
          reinterpret_cast<const std::uint8_t*>(command.verb.data()),
          command.verb.size(), digest + command.args.size());
    }
  }
  output.flush();
  std::cout << "digest=" << digest << "\n";
  return 0;
}
