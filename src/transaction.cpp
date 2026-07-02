#include "vesper/engine.hpp"

#include <cstring>

namespace vesper {

Result rename_document(Store& store, std::uint32_t id, std::string_view title) {
  const auto it = store.documents.find(id);
  if (it == store.documents.end()) {
    return Result::fail(Status::not_found, "rename source missing");
  }
  if (title.empty() || title.size() > 4096) {
    return Result::fail(Status::limit, "rename title invalid");
  }
  Document& document = *it->second;
  char* replacement = new char[title.size() + 1];
  std::memcpy(replacement, title.data(), title.size());
  replacement[title.size()] = '\0';
  delete[] document.title;
  document.title = replacement;
  document.title_size = title.size();
  ++store.epoch;
  store.events.push_back("document-renamed");
  return Result::success();
}

}  // namespace vesper
