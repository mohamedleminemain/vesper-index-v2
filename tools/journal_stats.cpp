#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

namespace {

std::vector<std::uint8_t> read_bytes(const char* path) {
  std::ifstream input(path, std::ios::binary);
  return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), {});
}

std::size_t script_payload_bytes(const std::vector<vesper::Command>& commands) {
  std::size_t total = 0;
  for (const auto& command : commands) {
    for (const auto& arg : command.args) total += arg.size();
  }
  return total;
}

std::size_t longest_verb(const std::vector<vesper::Command>& commands) {
  std::size_t best = 0;
  for (const auto& command : commands) {
    if (command.verb.size() > best) best = command.verb.size();
  }
  return best;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: vesper-journal-stats INPUT\n";
    return 2;
  }

  const auto bytes = read_bytes(argv[1]);
  std::vector<vesper::Command> commands;
  auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
  if (!parsed) {
    std::cerr << parsed.message << "\n";
    return 1;
  }

  std::map<std::string, std::size_t> counts;
  std::size_t total_args = 0;
  for (const auto& command : commands) {
    ++counts[command.verb];
    total_args += command.args.size();
  }

  std::cout << "commands=" << commands.size() << "\n";
  std::cout << "verbs=" << counts.size() << "\n";
  std::cout << "args=" << total_args << "\n";
  std::cout << "payload_chars=" << script_payload_bytes(commands) << "\n";
  std::cout << "longest_verb=" << longest_verb(commands) << "\n";
  for (const auto& [verb, count] : counts) {
    std::cout << verb << "=" << count << "\n";
  }
  return 0;
}
