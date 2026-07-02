#include "vesper/engine.hpp"

namespace vesper {

Result choose_palette(Store& store, std::uint32_t index) {
  if (index >= 16) {
    return Result::fail(Status::limit, "palette index invalid");
  }
  store.palette_plan.selected = index;
  store.events.push_back("palette-selected");
  return Result::success();
}

}  // namespace vesper
