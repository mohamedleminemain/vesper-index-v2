#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::uint32_t relation_edge_count(const Store& store, std::uint32_t node_id) {
  std::uint32_t count = 0;
  for (const auto& edge : store.relation.edges) {
    if (edge.from == node_id || edge.to == node_id) ++count;
  }
  return count;
}

std::uint32_t timeline_hit_count(const Store& store, std::uint32_t node_id) {
  std::uint32_t count = 0;
  for (const auto& event : store.timeline.events) {
    if (event.tag == node_id) ++count;
  }
  return count;
}

std::uint64_t digest_snapshot(const WatchSnapshot& snapshot) {
  std::uint64_t digest = 0xcbbb9d5dc1059ed8ULL;
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&snapshot.tag),
                      sizeof(snapshot.tag), digest);
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&snapshot.node_id),
                      sizeof(snapshot.node_id), digest);
  digest ^= snapshot.value;
  digest ^= static_cast<std::uint64_t>(snapshot.relation_edges) << 17U;
  digest ^= static_cast<std::uint64_t>(snapshot.timeline_hits) << 33U;
  return digest;
}

std::uint64_t summarize_watch_review(const WatchReviewState& review) {
  std::uint64_t digest = 0x3c6ef372fe94f82bULL;
  for (const auto& snapshot : review.snapshots) {
    digest ^= digest_snapshot(snapshot);
  }
  for (const auto& note : review.notes) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(note.data()),
                        note.size(), digest);
  }
  if (review.anchor.has_value()) digest ^= *review.anchor;
  return digest;
}

}  // namespace

Result snapshot_watches(Store& store, std::uint32_t tag) {
  if (tag == 0) {
    return Result::fail(Status::limit, "watch snapshot invalid");
  }
  store.watch_review.snapshots.clear();
  store.watch_review.notes.clear();
  store.watch_review.anchor = tag;
  for (const auto& node : store.nodes) {
    if (std::find(store.watches.begin(), store.watches.end(), node.get()) ==
        store.watches.end()) {
      continue;
    }
    WatchSnapshot snapshot;
    snapshot.tag = tag;
    snapshot.node_id = node->id;
    snapshot.value = node->value;
    snapshot.relation_edges = relation_edge_count(store, node->id);
    snapshot.timeline_hits = timeline_hit_count(store, node->id);
    snapshot.digest = digest_snapshot(snapshot);
    store.watch_review.snapshots.push_back(snapshot);
  }
  if (store.watch_review.snapshots.empty()) {
    return Result::fail(Status::invalid_state, "watch snapshot empty");
  }
  store.watch_review.notes.push_back(
      "snapshot-tag=" + std::to_string(tag) + ":count=" +
      std::to_string(store.watch_review.snapshots.size()));
  store.watch_review.digest = summarize_watch_review(store.watch_review);
  store.last_digest = store.watch_review.digest;
  store.events.push_back("watch-snapshot");
  return Result::success();
}

Result review_watch_state(Store& store, std::string_view mode) {
  if (store.watch_review.snapshots.empty()) {
    return Result::fail(Status::invalid_state, "watch review empty");
  }
  if (mode.empty() || mode.size() > 16) {
    return Result::fail(Status::limit, "watch review mode invalid");
  }
  store.watch_review.notes.clear();
  if (mode == "summary" || mode == "all") {
    for (const auto& snapshot : store.watch_review.snapshots) {
      store.watch_review.notes.push_back(
          "summary=" + std::to_string(snapshot.node_id) + ":value=" +
          std::to_string(snapshot.value));
    }
  }
  if (mode == "relations" || mode == "all") {
    for (const auto& snapshot : store.watch_review.snapshots) {
      store.watch_review.notes.push_back(
          "relations=" + std::to_string(snapshot.node_id) + ":" +
          std::to_string(snapshot.relation_edges));
    }
  }
  if (mode == "timeline" || mode == "all") {
    for (const auto& snapshot : store.watch_review.snapshots) {
      store.watch_review.notes.push_back(
          "timeline=" + std::to_string(snapshot.node_id) + ":" +
          std::to_string(snapshot.timeline_hits));
    }
  }
  if (store.watch_review.notes.empty()) {
    return Result::fail(Status::unsupported, "unknown watch review mode");
  }
  store.watch_review.digest = summarize_watch_review(store.watch_review);
  store.last_digest = store.watch_review.digest;
  store.events.push_back("watch-review");
  return Result::success();
}

}  // namespace vesper
