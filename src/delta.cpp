#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {

Result drop_node(Store& store, std::uint32_t id) {
  const auto it =
      std::find_if(store.nodes.begin(), store.nodes.end(),
                   [id](const auto& node) { return node->id == id; });
  if (it == store.nodes.end()) {
    return Result::fail(Status::not_found, "node missing");
  }
  store.nodes.erase(it);
  store.events.push_back("node-dropped");
  ++store.epoch;
  return Result::success();
}

}  // namespace vesper
