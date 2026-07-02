#include "vesper/engine.hpp"

#include <algorithm>
#include <cstring>

namespace vesper {

Document::Document(std::uint32_t value_id, std::string_view value_title,
                   std::vector<std::uint8_t> value_body)
    : id(value_id), title_size(value_title.size()), body(std::move(value_body)) {
  title = new char[title_size + 1];
  std::memcpy(title, value_title.data(), title_size);
  title[title_size] = '\0';
}

Document::~Document() { delete[] title; }

VectorPlan::~VectorPlan() { reset(); }

void VectorPlan::reset() {
  delete[] output;
  output = nullptr;
  allocated = 0;
  count = 0;
  width = 0;
  source.clear();
}

Store::Store() = default;

Store::~Store() {
  for (Key* key : active_keys) {
    delete key;
  }
  for (Key* key : retired_keys) {
    delete key;
  }
}

Result add_document(Store& store, std::uint32_t id, std::string_view title,
                    std::vector<std::uint8_t> body) {
  if (store.documents.find(id) != store.documents.end()) {
    return Result::fail(Status::invalid_state, "document already exists");
  }
  if (title.empty() || title.size() > 4096 || body.size() > (1U << 20U)) {
    return Result::fail(Status::limit, "document limits exceeded");
  }
  store.documents.emplace(
      id, std::make_unique<Document>(id, title, std::move(body)));
  store.events.push_back("document-added");
  ++store.epoch;
  return Result::success();
}

Result create_title_view(Store& store, std::uint32_t id) {
  const auto it = store.documents.find(id);
  if (it == store.documents.end()) {
    return Result::fail(Status::not_found, "view source missing");
  }
  store.title_views.push_back({id, it->second->title, it->second->title_size});
  store.events.push_back("view-created");
  return Result::success();
}

Result add_page(Store& store, std::uint32_t id,
                std::vector<std::uint8_t> bytes) {
  if (bytes.empty() || bytes.size() > (1U << 20U)) {
    return Result::fail(Status::limit, "invalid page size");
  }
  const auto duplicate =
      std::find_if(store.pages.begin(), store.pages.end(),
                   [id](const auto& page) { return page->id == id; });
  if (duplicate != store.pages.end()) {
    return Result::fail(Status::invalid_state, "duplicate page");
  }
  auto page = std::make_unique<Page>();
  page->id = id;
  page->bytes = std::move(bytes);
  page->generation = store.epoch;
  store.pages.push_back(std::move(page));
  store.events.push_back("page-added");
  return Result::success();
}

Result create_cursor(Store& store, std::uint32_t id, std::size_t offset) {
  const auto it =
      std::find_if(store.pages.begin(), store.pages.end(),
                   [id](const auto& page) { return page->id == id; });
  if (it == store.pages.end()) {
    return Result::fail(Status::not_found, "cursor page missing");
  }
  if (offset >= (*it)->bytes.size()) {
    return Result::fail(Status::limit, "cursor offset outside page");
  }
  store.cursors.push_back({id, it->get(), offset});
  store.events.push_back("cursor-created");
  return Result::success();
}

Result add_key(Store& store, std::uint32_t id,
               std::vector<std::uint8_t> material) {
  if (material.size() < 4 || material.size() > 256) {
    return Result::fail(Status::limit, "invalid key size");
  }
  const auto duplicate =
      std::find_if(store.active_keys.begin(), store.active_keys.end(),
                   [id](const Key* key) { return key && key->id == id; });
  if (duplicate != store.active_keys.end()) {
    return Result::fail(Status::invalid_state, "duplicate key");
  }
  Key* key = new Key();
  key->id = id;
  key->material_size = material.size();
  std::copy(material.begin(), material.end(), key->material.begin());
  store.active_keys.push_back(key);
  store.events.push_back("key-added");
  return Result::success();
}

Result add_node(Store& store, std::uint32_t id, std::uint64_t value) {
  const auto duplicate =
      std::find_if(store.nodes.begin(), store.nodes.end(),
                   [id](const auto& node) { return node->id == id; });
  if (duplicate != store.nodes.end()) {
    return Result::fail(Status::invalid_state, "duplicate node");
  }
  store.nodes.push_back(std::make_unique<Node>(Node{id, value}));
  store.events.push_back("node-added");
  return Result::success();
}

Result watch_node(Store& store, std::uint32_t id) {
  const auto it =
      std::find_if(store.nodes.begin(), store.nodes.end(),
                   [id](const auto& node) { return node->id == id; });
  if (it == store.nodes.end()) {
    return Result::fail(Status::not_found, "watch node missing");
  }
  store.watches.push_back(it->get());
  store.events.push_back("watch-added");
  return Result::success();
}

Result install_row(Store& store, std::vector<std::uint8_t> row) {
  if (!store.schema.verified) {
    return Result::fail(Status::invalid_state, "schema not verified");
  }
  if (row.size() != store.schema.row_size) {
    return Result::fail(Status::malformed, "row size mismatch");
  }
  store.row = std::move(row);
  store.events.push_back("row-installed");
  return Result::success();
}

}  // namespace vesper
