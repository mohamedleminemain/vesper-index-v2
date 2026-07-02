#include "vesper/engine.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

std::vector<std::uint8_t> read_bytes(const char* path) {
  std::ifstream input(path, std::ios::binary);
  return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), {});
}

void print_usage() {
  std::cerr << "usage: vesper-inspect INPUT [catalog|query|storage|security]\n";
}

std::string render_status(vesper::Status status) {
  switch (status) {
    case vesper::Status::ok:
      return "ok";
    case vesper::Status::malformed:
      return "malformed";
    case vesper::Status::unsupported:
      return "unsupported";
    case vesper::Status::invalid_state:
      return "invalid_state";
    case vesper::Status::not_found:
      return "not_found";
    case vesper::Status::limit:
      return "limit";
  }
  return "unknown";
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    print_usage();
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

  const auto inspected = vesper::inspect_store(store, argv[2]);
  if (!inspected) {
    std::cerr << inspected.message << "\n";
    return 1;
  }

  std::cout << "status=" << render_status(inspected.status) << "\n";
  std::cout << "digest=" << store.last_digest << "\n";
  for (const auto& line : store.inspect.lines) {
    std::cout << line << "\n";
  }
  std::cout << "events=" << store.events.size() << "\n";
  std::cout << "documents=" << store.documents.size() << "\n";
  std::cout << "pages=" << store.pages.size() << "\n";
  return 0;
}
