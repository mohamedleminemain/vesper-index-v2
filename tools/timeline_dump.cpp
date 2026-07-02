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
    std::cerr << "usage: vesper-timeline-dump INPUT\n";
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
  std::cout << "events=" << store.timeline.events.size() << "\n";
  std::cout << "window=" << store.timeline.window.size() << "\n";
  std::cout << "retained=" << store.timeline.retained << "\n";
  std::cout << "digest=" << store.timeline.digest << "\n";
  for (const auto& event : store.timeline.events) {
    std::cout << "event tag=" << event.tag
              << " stamp=" << event.stamp
              << " digest=" << event.digest << "\n";
  }
  for (const auto& event : store.timeline.window) {
    std::cout << "window tag=" << event.tag
              << " stamp=" << event.stamp
              << " digest=" << event.digest << "\n";
  }
  return 0;
}
