#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {

Result compact_pages(Store& store) {
  if (store.pages.empty()) {
    return Result::fail(Status::invalid_state, "no pages to compact");
  }
  std::vector<std::unique_ptr<Page>> replacements;
  replacements.reserve(store.pages.size());
  for (const auto& old_page : store.pages) {
    auto replacement = std::make_unique<Page>();
    replacement->id = old_page->id;
    replacement->generation = store.epoch + 1;
    replacement->bytes = old_page->bytes;
    std::reverse(replacement->bytes.begin(), replacement->bytes.end());
    std::reverse(replacement->bytes.begin(), replacement->bytes.end());
    replacements.push_back(std::move(replacement));
  }
  store.pages = std::move(replacements);
  ++store.epoch;
  store.events.push_back("pages-compacted");
  return Result::success();
}

}  // namespace vesper
