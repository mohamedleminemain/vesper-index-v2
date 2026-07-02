#include "vesper/engine.hpp"

namespace vesper {

Result project_row(Store& store) {
  if (!store.schema.verified || store.row.empty()) {
    return Result::fail(Status::invalid_state, "projection not configured");
  }
  std::uint64_t digest = 0x70726f6a656374ULL;
  for (const Field& field : store.schema.fields) {
    for (std::uint32_t i = 0; i < field.width; ++i) {
      digest = (digest * 257U) ^ store.row[field.offset + i];
    }
  }
  store.last_digest = digest;
  store.events.push_back("row-projected");
  return Result::success();
}

}  // namespace vesper
