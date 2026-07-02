#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {

Result rotate_key(Store& store, std::uint32_t old_id, std::uint32_t new_id,
                  std::vector<std::uint8_t> material) {
  if (material.size() < 4 || material.size() > 256) {
    return Result::fail(Status::limit, "replacement key invalid");
  }
  const auto old =
      std::find_if(store.active_keys.begin(), store.active_keys.end(),
                   [old_id](const Key* key) {
                     return key && key->id == old_id;
                   });
  if (old == store.active_keys.end()) {
    return Result::fail(Status::not_found, "old key missing");
  }
  Key* replacement = new Key();
  replacement->id = new_id;
  replacement->material_size = material.size();
  std::copy(material.begin(), material.end(), replacement->material.begin());
  store.retired_keys.push_back(*old);
  store.active_keys.push_back(replacement);
  store.events.push_back("key-rotated");
  return Result::success();
}

}  // namespace vesper
