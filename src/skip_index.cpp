#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::vector<SkipIndexPoint> build_base_points(Store& store,
                                              std::uint32_t stride) {
  std::vector<SkipIndexPoint> points;
  for (const auto& page : store.pages) {
    if (page->bytes.empty()) continue;
    for (std::size_t offset = 0; offset < page->bytes.size(); offset += stride) {
      SkipIndexPoint point;
      point.key = static_cast<std::uint32_t>(
          page->bytes[offset] + ((offset & 0xffU) << 8U) + (page->id << 16U));
      point.page_id = page->id;
      point.offset = static_cast<std::uint32_t>(offset);
      point.span = static_cast<std::uint32_t>(
          std::min<std::size_t>(stride, page->bytes.size() - offset));
      points.push_back(point);
    }
  }
  std::sort(points.begin(), points.end(),
            [](const auto& left, const auto& right) {
              if (left.key != right.key) return left.key < right.key;
              if (left.page_id != right.page_id) return left.page_id < right.page_id;
              return left.offset < right.offset;
            });
  return points;
}

SkipIndexLayer compact_layer(const SkipIndexLayer& layer, std::uint32_t stride) {
  SkipIndexLayer next;
  if (layer.points.empty()) return next;
  for (std::size_t i = 0; i < layer.points.size(); i += stride) {
    SkipIndexPoint point = layer.points[i];
    point.span = std::min<std::uint32_t>(
        stride, static_cast<std::uint32_t>(layer.points.size() - i));
    next.points.push_back(point);
  }
  return next;
}

std::uint64_t digest_skip(const SkipIndexState& state) {
  std::uint64_t digest = 0x3c6ef372fe94f82bULL ^ state.stride;
  for (const auto& layer : state.layers) {
    for (const auto& point : layer.points) {
      digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&point.key),
                          sizeof(point.key), digest);
      digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&point.page_id),
                          sizeof(point.page_id), digest);
      digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&point.offset),
                          sizeof(point.offset), digest);
    }
  }
  if (state.last_match.has_value()) {
    digest ^= state.last_match->key;
    digest ^= static_cast<std::uint64_t>(state.last_match->page_id) << 17U;
  }
  return digest;
}

}  // namespace

Result build_skip_index(Store& store, std::uint32_t stride,
                        std::uint32_t levels) {
  if (stride == 0 || stride > 64 || levels == 0 || levels > 8) {
    return Result::fail(Status::limit, "skip parameters invalid");
  }
  auto base = build_base_points(store, stride);
  if (base.empty()) {
    return Result::fail(Status::invalid_state, "skip source empty");
  }
  store.skip.layers.clear();
  store.skip.layers.push_back({std::move(base)});
  store.skip.stride = stride;
  store.skip.last_match.reset();
  for (std::uint32_t level = 1; level < levels; ++level) {
    SkipIndexLayer next = compact_layer(store.skip.layers.back(), stride);
    if (next.points.empty()) break;
    store.skip.layers.push_back(std::move(next));
    if (store.skip.layers.back().points.size() <= 1) break;
  }
  store.skip.built = true;
  store.last_digest = digest_skip(store.skip);
  store.events.push_back("skip-build");
  return Result::success();
}

Result seek_skip_index(Store& store, std::uint32_t key) {
  if (!store.skip.built || store.skip.layers.empty()) {
    return Result::fail(Status::invalid_state, "skip index not built");
  }
  store.skip.search_key = key;
  std::optional<SkipIndexPoint> candidate;
  for (std::size_t layer_index = store.skip.layers.size(); layer_index-- > 0;) {
    const auto& layer = store.skip.layers[layer_index].points;
    const auto it = std::lower_bound(
        layer.begin(), layer.end(), key,
        [](const SkipIndexPoint& point, std::uint32_t needle) {
          return point.key < needle;
        });
    if (it != layer.end()) {
      candidate = *it;
    } else if (!layer.empty()) {
      candidate = layer.back();
    }
  }
  if (!candidate.has_value()) {
    return Result::fail(Status::not_found, "skip candidate missing");
  }
  store.skip.last_match = candidate;
  store.last_digest = digest_skip(store.skip) ^
                      (static_cast<std::uint64_t>(candidate->offset) << 32U);
  store.events.push_back("skip-seek");
  return Result::success();
}

}  // namespace vesper
