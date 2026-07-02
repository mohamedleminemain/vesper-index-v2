#include "vesper/engine.hpp"

namespace vesper {

Result render_label(Store& store) {
  if (store.label_plan.text.empty()) {
    return Result::fail(Status::invalid_state, "label missing");
  }
  volatile char display[32];
  std::uint64_t digest = 0x72656e646572ULL;
  for (std::uint32_t i = 0; i < store.label_plan.declared_size; ++i) {
    display[i] = store.label_plan.text[i];
    digest = (digest * 131U) ^
             static_cast<unsigned char>(display[i]);
  }
  store.last_digest = digest;
  store.events.push_back("label-rendered");
  return Result::success();
}

}  // namespace vesper
