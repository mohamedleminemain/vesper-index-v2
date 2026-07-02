#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::uint64_t digest_timeline(const TimelineState& timeline) {
  std::uint64_t digest = 0xa54ff53a5f1d36f1ULL;
  for (const auto& event : timeline.events) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&event.tag),
                        sizeof(event.tag), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&event.stamp),
                        sizeof(event.stamp), digest);
  }
  for (const auto& event : timeline.window) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&event.digest),
                        sizeof(event.digest), digest);
  }
  digest ^= timeline.retained;
  return digest;
}

}  // namespace

Result record_timeline(Store& store, std::uint32_t tag, std::uint64_t stamp) {
  if (tag == 0 || stamp == 0) {
    return Result::fail(Status::limit, "timeline event invalid");
  }
  TimelineEvent event;
  event.tag = tag;
  event.stamp = stamp;
  event.digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&tag),
                            sizeof(tag), stamp ^ store.last_digest);
  store.timeline.events.push_back(event);
  std::sort(store.timeline.events.begin(), store.timeline.events.end(),
            [](const auto& left, const auto& right) {
              if (left.stamp != right.stamp) return left.stamp < right.stamp;
              return left.tag < right.tag;
            });
  store.timeline.digest = digest_timeline(store.timeline);
  store.last_digest = store.timeline.digest;
  store.events.push_back("timeline-record");
  return Result::success();
}

Result window_timeline(Store& store, std::uint64_t begin, std::uint64_t end) {
  if (begin > end) {
    return Result::fail(Status::malformed, "timeline window invalid");
  }
  store.timeline.window.clear();
  for (const auto& event : store.timeline.events) {
    if (event.stamp >= begin && event.stamp <= end) {
      store.timeline.window.push_back(event);
    }
  }
  if (store.timeline.window.empty()) {
    return Result::fail(Status::not_found, "timeline window empty");
  }
  store.timeline.digest = digest_timeline(store.timeline);
  store.last_digest = store.timeline.digest ^ begin ^ (end << 1U);
  store.events.push_back("timeline-window");
  return Result::success();
}

Result retain_timeline(Store& store, std::uint32_t count) {
  if (count == 0) {
    return Result::fail(Status::limit, "timeline retain invalid");
  }
  if (store.timeline.events.size() > count) {
    const std::size_t drop = store.timeline.events.size() - count;
    store.timeline.events.erase(store.timeline.events.begin(),
                                store.timeline.events.begin() +
                                    static_cast<std::ptrdiff_t>(drop));
    store.timeline.retained += drop;
  }
  store.timeline.digest = digest_timeline(store.timeline);
  store.last_digest = store.timeline.digest;
  store.events.push_back("timeline-retain");
  return Result::success();
}

}  // namespace vesper
