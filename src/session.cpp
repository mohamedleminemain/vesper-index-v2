#include "vesper/engine.hpp"

namespace vesper {

Result configure_signatures(Store& store, std::uint32_t allocated,
                            std::uint32_t requested) {
  if (allocated == 0 || allocated > 1024 || requested == 0 ||
      requested > 1024) {
    return Result::fail(Status::limit, "signature plan limits exceeded");
  }
  if (requested > allocated) {
    return Result::fail(Status::limit, "signature plan limits exceeded");
  }
  store.signatures.slots.assign(allocated, 0);
  store.signatures.requested_slots = requested;
  store.signatures.finalized = false;
  store.events.push_back("signatures-configured");
  return Result::success();
}

Result finalize_session(Store& store) {
  if (store.signatures.slots.empty()) {
    return Result::fail(Status::invalid_state, "signature slots missing");
  }
  store.signatures.finalized = true;
  store.events.push_back("session-finalized");
  return Result::success();
}

}  // namespace vesper
