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

}  // namespace

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: vesper-snapshot-dump INPUT\n";
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

  std::cout << "snapshots=" << store.snapshots.captured.size() << "\n";
  std::cout << "current=";
  if (store.snapshots.current.has_value()) {
    std::cout << *store.snapshots.current;
  } else {
    std::cout << "none";
  }
  std::cout << "\n";
  std::cout << "restores=" << store.snapshots.restore_count << "\n";
  for (const auto& snapshot : store.snapshots.captured) {
    std::cout << "snapshot " << snapshot.snapshot_id
              << " docs=" << snapshot.documents.size()
              << " pages=" << snapshot.pages.size()
              << " nodes=" << snapshot.nodes.size()
              << " digest=" << snapshot.digest << "\n";
    for (const auto& doc : snapshot.documents) {
      std::cout << "  doc " << doc.id << " title=" << doc.title
                << " body=" << doc.body.size() << "\n";
    }
    for (const auto& page : snapshot.pages) {
      std::cout << "  page " << page.id
                << " bytes=" << page.bytes.size()
                << " generation=" << page.generation << "\n";
    }
  }
  return 0;
}
