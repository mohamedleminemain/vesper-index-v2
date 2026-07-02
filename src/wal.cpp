#include "vesper/engine.hpp"

#include <algorithm>
#include <string_view>

namespace vesper {
namespace {

bool valid_segment_kind(std::string_view kind) {
  if (kind.empty() || kind.size() > 32) return false;
  return std::all_of(kind.begin(), kind.end(), [](char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') ||
           ch == '_' || ch == '-';
  });
}

std::uint64_t entry_digest(const SegmentEntry& entry, std::uint64_t seed) {
  std::uint64_t digest = hash_bytes(
      reinterpret_cast<const std::uint8_t*>(entry.kind.data()), entry.kind.size(),
      seed ^ entry.ordinal);
  if (!entry.payload.empty()) {
    digest = hash_bytes(entry.payload.data(), entry.payload.size(), digest);
  }
  digest ^= static_cast<std::uint64_t>(entry.payload.size()) << 11U;
  digest ^= entry.checksum << 1U;
  return digest;
}

SegmentImage* find_segment(Store& store, std::uint32_t id) {
  for (auto& image : store.segments.sealed) {
    if (image.segment_id == id) return &image;
  }
  return nullptr;
}

void clear_open_segment(Store& store) {
  store.segments.active_segment = 0;
  store.segments.expected_entries = 0;
  store.segments.open = false;
  store.segments.pending.clear();
}

void materialize_segment_effect(Store& store, const SegmentEntry& entry) {
  std::string marker = entry.kind;
  marker.push_back(':');
  marker += std::to_string(entry.payload.size());
  store.events.push_back(marker);
  store.last_digest = entry.digest;
  ++store.epoch;
}

std::uint64_t summarize_segment(const SegmentImage& image) {
  std::uint64_t digest = hash_bytes(
      reinterpret_cast<const std::uint8_t*>(&image.segment_id),
      sizeof(image.segment_id), 0xcbf29ce484222325ULL);
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&image.expected_entries),
                      sizeof(image.expected_entries), digest);
  for (const SegmentEntry& entry : image.entries) {
    digest = entry_digest(entry, digest);
  }
  return digest;
}

}  // namespace

Result begin_segment(Store& store, std::uint32_t segment_id,
                     std::uint32_t expected_entries) {
  if (segment_id == 0 || expected_entries == 0 || expected_entries > 512) {
    return Result::fail(Status::limit, "segment limits exceeded");
  }
  if (store.segments.open) {
    return Result::fail(Status::invalid_state, "segment already open");
  }
  if (find_segment(store, segment_id) != nullptr) {
    return Result::fail(Status::invalid_state, "segment already sealed");
  }
  store.segments.active_segment = segment_id;
  store.segments.expected_entries = expected_entries;
  store.segments.sequence = 0;
  store.segments.open = true;
  store.segments.pending.clear();
  store.segments.positions.clear();
  store.events.push_back("segment-begin");
  return Result::success();
}

Result append_segment(Store& store, std::string_view kind,
                      std::vector<std::uint8_t> payload) {
  if (!store.segments.open) {
    return Result::fail(Status::invalid_state, "segment not open");
  }
  if (!valid_segment_kind(kind) || payload.size() > 8192) {
    return Result::fail(Status::limit, "invalid segment entry");
  }
  if (store.segments.pending.size() >= store.segments.expected_entries) {
    return Result::fail(Status::limit, "segment entry limit reached");
  }
  SegmentEntry entry;
  entry.ordinal = static_cast<std::uint32_t>(store.segments.sequence++);
  entry.kind.assign(kind);
  entry.payload = std::move(payload);
  entry.checksum = hash_bytes(entry.payload.data(), entry.payload.size(),
                              0x9e3779b97f4a7c15ULL ^ entry.ordinal);
  entry.digest = entry_digest(entry, store.last_digest ^ store.epoch);
  store.segments.positions[entry.ordinal] = store.segments.pending.size();
  store.segments.pending.push_back(std::move(entry));
  store.last_digest = store.segments.pending.back().digest;
  store.events.push_back("segment-append");
  return Result::success();
}

Result seal_segment(Store& store) {
  if (!store.segments.open) {
    return Result::fail(Status::invalid_state, "segment not open");
  }
  if (store.segments.pending.size() != store.segments.expected_entries) {
    return Result::fail(Status::invalid_state, "segment entry count mismatch");
  }
  SegmentImage image;
  image.segment_id = store.segments.active_segment;
  image.expected_entries = store.segments.expected_entries;
  image.entries = store.segments.pending;
  image.seal_digest = summarize_segment(image);
  store.segments.replay_history.push_back(image.seal_digest);
  store.last_digest = image.seal_digest;
  store.segments.sealed.push_back(std::move(image));
  clear_open_segment(store);
  store.events.push_back("segment-seal");
  return Result::success();
}

Result replay_segment(Store& store, std::uint32_t segment_id) {
  SegmentImage* image = find_segment(store, segment_id);
  if (image == nullptr) {
    return Result::fail(Status::not_found, "segment missing");
  }
  if (image->entries.empty()) {
    return Result::fail(Status::invalid_state, "segment empty");
  }
  std::uint64_t digest = 0x84222325cbf29ce4ULL ^ image->segment_id;
  for (const SegmentEntry& entry : image->entries) {
    digest = entry_digest(entry, digest);
    materialize_segment_effect(store, entry);
  }
  digest ^= image->seal_digest;
  store.segments.replay_history.push_back(digest);
  store.last_digest = digest;
  store.events.push_back("segment-replay");
  return Result::success();
}

}  // namespace vesper
