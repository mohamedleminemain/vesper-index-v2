#include "vesper/engine.hpp"

namespace vesper {

Result define_links(Store& store, std::vector<std::uint32_t> targets) {
  if (targets.empty() || targets.size() > 4096) {
    return Result::fail(Status::limit, "link table limits exceeded");
  }
  store.links.targets = std::move(targets);
  store.links.selected.reset();
  store.events.push_back("links-defined");
  return Result::success();
}

Result select_link(Store& store, std::uint32_t index) {
  if (index >= store.links.targets.size()) {
    return Result::fail(Status::limit, "link selection outside table");
  }
  store.links.selected = index;
  store.events.push_back("link-selected");
  return Result::success();
}

}  // namespace vesper
