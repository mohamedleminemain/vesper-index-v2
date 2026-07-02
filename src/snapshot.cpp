#include "vesper/engine.hpp"

#include <algorithm>
#include <memory>
#include <string_view>

namespace vesper {
namespace {

SnapshotView* find_snapshot(Store& store, std::uint32_t id) {
  for (auto& snapshot : store.snapshots.captured) {
    if (snapshot.snapshot_id == id) return &snapshot;
  }
  return nullptr;
}

std::uint64_t digest_title_pairs(
    const std::vector<std::pair<std::uint32_t, std::string>>& titles) {
  std::uint64_t digest = 0x517cc1b727220a95ULL;
  for (const auto& pair : titles) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&pair.first),
                        sizeof(pair.first), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(pair.second.data()),
                        pair.second.size(), digest);
  }
  return digest;
}

std::uint64_t digest_snapshot(const SnapshotView& snapshot) {
  std::uint64_t digest = hash_bytes(
      reinterpret_cast<const std::uint8_t*>(&snapshot.snapshot_id),
      sizeof(snapshot.snapshot_id), 0x6a09e667f3bcc909ULL);
  for (const auto& doc : snapshot.documents) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&doc.id),
                        sizeof(doc.id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(doc.title.data()),
                        doc.title.size(), digest);
    digest = hash_bytes(doc.body.data(), doc.body.size(), digest);
  }
  for (const auto& page : snapshot.pages) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&page.id),
                        sizeof(page.id), digest);
    digest = hash_bytes(page.bytes.data(), page.bytes.size(), digest);
  }
  for (const auto& node : snapshot.nodes) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&node.first),
                        sizeof(node.first), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&node.second),
                        sizeof(node.second), digest);
  }
  digest ^= digest_title_pairs(snapshot.titles);
  return digest;
}

void clear_live_store(Store& store) {
  store.documents.clear();
  store.title_views.clear();
  store.pages.clear();
  store.cursors.clear();
  store.nodes.clear();
  store.watches.clear();
}

}  // namespace

Result capture_snapshot(Store& store, std::uint32_t snapshot_id) {
  if (snapshot_id == 0) {
    return Result::fail(Status::limit, "snapshot id invalid");
  }
  if (find_snapshot(store, snapshot_id) != nullptr) {
    return Result::fail(Status::invalid_state, "snapshot already exists");
  }
  SnapshotView view;
  view.snapshot_id = snapshot_id;
  view.documents.reserve(store.documents.size());
  for (const auto& item : store.documents) {
    SnapshotDocument doc;
    doc.id = item.first;
    doc.title.assign(item.second->title, item.second->title_size);
    doc.body = item.second->body;
    view.documents.push_back(std::move(doc));
    view.titles.emplace_back(item.first, view.documents.back().title);
  }
  std::sort(view.documents.begin(), view.documents.end(),
            [](const auto& left, const auto& right) {
              return left.id < right.id;
            });
  std::sort(view.titles.begin(), view.titles.end(),
            [](const auto& left, const auto& right) {
              return left.first < right.first;
            });
  view.pages.reserve(store.pages.size());
  for (const auto& page : store.pages) {
    view.pages.push_back({page->id, page->bytes, page->generation});
  }
  std::sort(view.pages.begin(), view.pages.end(),
            [](const auto& left, const auto& right) {
              return left.id < right.id;
            });
  for (const auto& node : store.nodes) {
    view.nodes.emplace_back(node->id, node->value);
  }
  std::sort(view.nodes.begin(), view.nodes.end(),
            [](const auto& left, const auto& right) {
              return left.first < right.first;
            });
  view.digest = digest_snapshot(view);
  store.snapshots.captured.push_back(std::move(view));
  store.snapshots.current = snapshot_id;
  store.last_digest = store.snapshots.captured.back().digest;
  store.events.push_back("snapshot-capture");
  return Result::success();
}

Result restore_snapshot(Store& store, std::uint32_t snapshot_id) {
  SnapshotView* snapshot = find_snapshot(store, snapshot_id);
  if (snapshot == nullptr) {
    return Result::fail(Status::not_found, "snapshot missing");
  }
  clear_live_store(store);
  for (const auto& doc : snapshot->documents) {
    auto live = std::make_unique<Document>(doc.id, doc.title, doc.body);
    store.documents.emplace(doc.id, std::move(live));
  }
  for (const auto& title : snapshot->titles) {
    const auto it = store.documents.find(title.first);
    if (it != store.documents.end()) {
      store.title_views.push_back({title.first, it->second->title,
                                   it->second->title_size});
    }
  }
  for (const auto& page : snapshot->pages) {
    auto live = std::make_unique<Page>();
    live->id = page.id;
    live->bytes = page.bytes;
    live->generation = page.generation;
    store.pages.push_back(std::move(live));
  }
  for (const auto& node : snapshot->nodes) {
    store.nodes.push_back(std::make_unique<Node>(Node{node.first, node.second}));
  }
  store.snapshots.current = snapshot_id;
  ++store.snapshots.restore_count;
  store.last_digest = digest_snapshot(*snapshot) ^ store.snapshots.restore_count;
  store.events.push_back("snapshot-restore");
  return Result::success();
}

Result inspect_snapshots(Store& store) {
  if (store.snapshots.captured.empty()) {
    return Result::fail(Status::not_found, "snapshot catalog empty");
  }
  std::uint64_t digest = 0x7f4a7c159e3779b9ULL;
  for (const auto& snapshot : store.snapshots.captured) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&snapshot.snapshot_id),
                        sizeof(snapshot.snapshot_id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&snapshot.digest),
                        sizeof(snapshot.digest), digest);
  }
  if (store.snapshots.current.has_value()) {
    digest ^= *store.snapshots.current;
  }
  store.inspect.lines.push_back("snapshots=" +
                                std::to_string(store.snapshots.captured.size()));
  store.inspect.lines.push_back("restores=" +
                                std::to_string(store.snapshots.restore_count));
  store.last_digest = digest;
  store.inspect.digest = digest;
  store.events.push_back("snapshot-inspect");
  return Result::success();
}

}  // namespace vesper
