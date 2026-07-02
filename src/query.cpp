#include "vesper/engine.hpp"

namespace vesper {

Result scan_cursor(Store& store, std::size_t cursor_index) {
  if (cursor_index >= store.cursors.size()) {
    return Result::fail(Status::not_found, "cursor missing");
  }
  const Cursor& cursor = store.cursors[cursor_index];
  const Page* page = cursor.page;
  std::uint64_t digest = page->id ^ page->generation;
  for (std::size_t i = cursor.offset; i < page->bytes.size(); ++i) {
    digest = (digest * 131U) ^ page->bytes[i];
  }
  store.last_digest = digest;
  store.events.push_back("cursor-scanned");
  return Result::success();
}

}  // namespace vesper
