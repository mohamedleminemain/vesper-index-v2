#include "vesper/engine.hpp"

namespace vesper {

Result audit_watches(Store& store) {
  std::uint64_t digest = 0xa4d17c3e2b1f0955ULL;
  for (const Node* node : store.watches) {
    digest ^= node->value + (static_cast<std::uint64_t>(node->id) << 32U);
    digest = (digest << 13U) | (digest >> 51U);
  }
  store.last_digest = digest;
  store.events.push_back("watches-audited");
  return Result::success();
}

}  // namespace vesper
