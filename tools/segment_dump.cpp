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
    std::cerr << "usage: vesper-segment-dump INPUT\n";
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

  std::cout << "sealed=" << store.segments.sealed.size() << "\n";
  std::cout << "history=" << store.segments.replay_history.size() << "\n";
  for (const auto& image : store.segments.sealed) {
    std::cout << "segment " << image.segment_id
              << " expected=" << image.expected_entries
              << " entries=" << image.entries.size()
              << " seal=" << image.seal_digest << "\n";
    for (const auto& entry : image.entries) {
      std::cout << "  #" << entry.ordinal
                << " kind=" << entry.kind
                << " payload=" << entry.payload.size()
                << " checksum=" << entry.checksum
                << " digest=" << entry.digest << "\n";
    }
  }
  return 0;
}
