#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

Page* find_page(Store& store, std::uint32_t page_id) {
  for (const auto& page : store.pages) {
    if (page->id == page_id) return page.get();
  }
  return nullptr;
}

void touch_slot(CacheState& cache, std::size_t slot) {
  cache.lru.erase(std::remove(cache.lru.begin(), cache.lru.end(), slot),
                  cache.lru.end());
  cache.lru.push_back(slot);
}

std::size_t measure_cache(const CacheState& cache) {
  std::size_t total = 0;
  for (const auto& window : cache.windows) {
    total += window.bytes.size();
  }
  return total;
}

std::uint64_t digest_cache(const CacheState& cache) {
  std::uint64_t digest = 0xbb67ae8584caa73bULL ^ cache.target_bytes;
  for (const auto& window : cache.windows) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&window.page_id),
                        sizeof(window.page_id), digest);
    digest = hash_bytes(window.bytes.data(), window.bytes.size(), digest);
  }
  if (cache.last_window.has_value()) {
    digest ^= static_cast<std::uint64_t>(*cache.last_window) << 9U;
  }
  digest ^= cache.evictions << 1U;
  return digest;
}

bool evict_one(CacheState& cache) {
  while (!cache.lru.empty()) {
    const std::size_t slot = cache.lru.front();
    cache.lru.pop_front();
    if (slot >= cache.windows.size()) continue;
    if (cache.windows[slot].pinned) continue;
    cache.total_bytes -= cache.windows[slot].bytes.size();
    cache.windows.erase(cache.windows.begin() + static_cast<std::ptrdiff_t>(slot));
    ++cache.evictions;
    for (std::size_t& index : cache.lru) {
      if (index > slot) --index;
    }
    return true;
  }
  return false;
}

}  // namespace

Result cache_page_window(Store& store, std::uint32_t page_id,
                         std::uint32_t width) {
  Page* page = find_page(store, page_id);
  if (page == nullptr) {
    return Result::fail(Status::not_found, "cache page missing");
  }
  if (width == 0 || width > page->bytes.size()) {
    return Result::fail(Status::limit, "cache width invalid");
  }
  CacheWindow window;
  window.page_id = page_id;
  window.offset = page->bytes.size() > width ? page->bytes.size() - width : 0;
  window.bytes.insert(window.bytes.end(),
                      page->bytes.begin() + static_cast<std::ptrdiff_t>(window.offset),
                      page->bytes.begin() + static_cast<std::ptrdiff_t>(window.offset + width));
  window.stamp = ++store.cache.stamp;
  window.hits = 1;
  window.pinned = (page_id % 2U) == 0;
  store.cache.windows.push_back(std::move(window));
  store.cache.last_window = store.cache.windows.size() - 1;
  touch_slot(store.cache, *store.cache.last_window);
  store.cache.total_bytes = measure_cache(store.cache);
  while (store.cache.total_bytes > store.cache.target_bytes && evict_one(store.cache)) {
    store.cache.total_bytes = measure_cache(store.cache);
  }
  store.last_digest = digest_cache(store.cache);
  store.events.push_back("cache-window");
  return Result::success();
}

Result touch_cache_window(Store& store, std::uint32_t slot) {
  if (slot >= store.cache.windows.size()) {
    return Result::fail(Status::limit, "cache slot invalid");
  }
  CacheWindow& window = store.cache.windows[slot];
  ++window.hits;
  window.stamp = ++store.cache.stamp;
  touch_slot(store.cache, slot);
  store.cache.last_window = slot;
  store.last_digest = digest_cache(store.cache) ^
                      (static_cast<std::uint64_t>(window.hits) << 19U);
  store.events.push_back("cache-touch");
  return Result::success();
}

Result evict_cache_windows(Store& store, std::uint32_t target_bytes) {
  store.cache.target_bytes = target_bytes;
  store.cache.total_bytes = measure_cache(store.cache);
  while (store.cache.total_bytes > store.cache.target_bytes) {
    if (!evict_one(store.cache)) {
      return Result::fail(Status::invalid_state, "cache only has pinned windows");
    }
    store.cache.total_bytes = measure_cache(store.cache);
  }
  store.last_digest = digest_cache(store.cache);
  store.events.push_back("cache-evict");
  return Result::success();
}

}  // namespace vesper
