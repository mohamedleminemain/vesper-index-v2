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
  if (argc != 2) return 2;

  const auto bytes = read_bytes(argv[1]);
  std::vector<vesper::Command> commands;
  const auto parsed = vesper::parse_script(bytes.data(), bytes.size(), commands);
  if (!parsed) return 1;

  vesper::Store store;
  const auto executed =
      vesper::execute_commands(store, commands, vesper::Profile::all);
  if (!executed) return 1;

  std::cout << "last_digest=" << store.last_digest << "\n";
  std::cout << "events=" << store.events.size() << "\n";
  std::cout << "digests=" << store.digests.entries.size() << "\n";
  std::cout << "checkpoints=" << store.checkpoints.entries.size() << "\n";
  std::cout << "tickets=" << store.tickets.tickets.size() << "\n";
  std::cout << "ledger=" << store.ledger.entries.size() << "\n";
  std::cout << "quota=" << store.quota.buckets.size() << "\n";
  std::cout << "relations=" << store.relation.edges.size() << "\n";
  std::cout << "sample_rows=" << store.sample.rows.size() << "\n";
  std::cout << "policy_rules=" << store.policy.rules.size() << "\n";
  return 0;
}
