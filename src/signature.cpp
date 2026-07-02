#include "vesper/engine.hpp"

namespace vesper {

Result write_signatures(Store& store) {
  if (!store.signatures.finalized) {
    return Result::fail(Status::invalid_state, "session not finalized");
  }
  for (std::uint32_t i = 0; i < store.signatures.requested_slots; ++i) {
    store.signatures.slots[i] =
        store.last_digest ^ (0x9e3779b97f4a7c15ULL * (i + 1U));
  }
  store.events.push_back("signatures-written");
  return Result::success();
}

}  // namespace vesper
