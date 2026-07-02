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
    std::cerr << "usage: vesper-plan-dump INPUT\n";
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
  std::cout << "mode=" << store.plan.mode << "\n";
  std::cout << "steps=" << store.plan.steps.size() << "\n";
  std::cout << "cost=" << store.plan.total_cost << "\n";
  std::cout << "limit=" << store.plan.limit << "\n";
  std::cout << "result=" << store.plan.result_digest << "\n";
  for (const auto& step : store.plan.steps) {
    std::cout << step.name
              << " cost=" << step.estimated_cost
              << " fanout=" << step.fanout
              << " cardinality=" << step.cardinality
              << " blocking=" << (step.blocking ? 1 : 0) << "\n";
  }
  return 0;
}
