#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

SnapshotView* find_snapshot(Store& store, std::uint32_t id) {
  for (auto& snapshot : store.snapshots.captured) {
    if (snapshot.snapshot_id == id) return &snapshot;
  }
  return nullptr;
}

std::uint64_t summarize_diff(const DiffState& diff) {
  std::uint64_t digest = 0xa54ff53a5f1d36f1ULL ^ diff.left_id ^ (diff.right_id << 8U);
  for (const auto& entry : diff.entries) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(entry.field.data()),
                        entry.field.size(), digest);
    digest ^= static_cast<std::uint64_t>(entry.left - entry.right);
  }
  return digest ^ static_cast<std::uint64_t>(diff.ready);
}

}  // namespace

Result diff_snapshots(Store& store, std::uint32_t left_id, std::uint32_t right_id) {
  SnapshotView* left = find_snapshot(store, left_id);
  SnapshotView* right = find_snapshot(store, right_id);
  if (left == nullptr || right == nullptr) {
    return Result::fail(Status::not_found, "snapshot diff source missing");
  }
  store.diff.left_id = left_id;
  store.diff.right_id = right_id;
  store.diff.entries.clear();
  store.diff.entries.push_back({"documents",
                                static_cast<std::int64_t>(left->documents.size()),
                                static_cast<std::int64_t>(right->documents.size())});
  store.diff.entries.push_back({"pages",
                                static_cast<std::int64_t>(left->pages.size()),
                                static_cast<std::int64_t>(right->pages.size())});
  store.diff.entries.push_back({"nodes",
                                static_cast<std::int64_t>(left->nodes.size()),
                                static_cast<std::int64_t>(right->nodes.size())});
  store.diff.entries.push_back({"titles",
                                static_cast<std::int64_t>(left->titles.size()),
                                static_cast<std::int64_t>(right->titles.size())});
  store.diff.entries.push_back({"digest",
                                static_cast<std::int64_t>(left->digest),
                                static_cast<std::int64_t>(right->digest)});
  store.diff.ready = true;
  store.diff.digest = summarize_diff(store.diff);
  store.last_digest = store.diff.digest;
  store.events.push_back("snapshot-diff");
  return Result::success();
}

}  // namespace vesper
