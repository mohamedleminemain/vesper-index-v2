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
    std::cerr << "usage: vesper-metrics-dump INPUT\n";
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
  std::cout << "scope=" << store.metrics.scope << "\n";
  std::cout << "samples=" << store.metrics.samples.size() << "\n";
  std::cout << "buckets=" << store.metrics.buckets.size() << "\n";
  std::cout << "min=" << store.metrics.minimum << "\n";
  std::cout << "max=" << store.metrics.maximum << "\n";
  std::cout << "avg=" << store.metrics.average << "\n";
  std::cout << "reduced=" << store.metrics.reduced << "\n";
  for (const auto& bucket : store.metrics.buckets) {
    std::cout << "bucket " << bucket.lower
              << "-" << bucket.upper
              << " count=" << bucket.count
              << " weight=" << bucket.weight << "\n";
  }
  return 0;
}
