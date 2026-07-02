#include "vesper/engine.hpp"

namespace vesper {

Result export_selected_link(Store& store) {
  if (!store.links.selected) {
    return Result::fail(Status::invalid_state, "no selected link");
  }
  const std::uint32_t index = *store.links.selected;
  const std::uint32_t target = store.links.targets[index];
  std::uint64_t digest = 0x6578706f7274ULL ^ target;
  digest *= 0x9e3779b185ebca87ULL;
  store.last_digest = digest;
  store.events.push_back("link-exported");
  return Result::success();
}

}  // namespace vesper
