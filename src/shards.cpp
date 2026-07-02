#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::uint64_t digest_shards(const ShardState& shards) {
  std::uint64_t digest = 0x34e90c6cc0ac29b7ULL ^ shards.count;
  for (const auto& entry : shards.placements) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.shard),
                        sizeof(entry.shard), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.object_id),
                        sizeof(entry.object_id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.weight),
                        sizeof(entry.weight), digest);
  }
  for (std::uint64_t load : shards.load) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&load),
                        sizeof(load), digest);
  }
  return digest;
}

std::uint32_t lightest_shard(const std::vector<std::uint64_t>& load) {
  std::uint32_t best = 0;
  for (std::uint32_t i = 1; i < load.size(); ++i) {
    if (load[i] < load[best]) best = i;
  }
  return best;
}

}  // namespace

Result assign_shards(Store& store, std::uint32_t count) {
  if (count == 0 || count > 64) {
    return Result::fail(Status::limit, "shard count invalid");
  }
  store.shards.placements.clear();
  store.shards.load.assign(count, 0);
  store.shards.count = count;
  for (const auto& item : store.documents) {
    const std::uint32_t shard = item.first % count;
    const std::uint64_t weight = item.second->title_size + item.second->body.size();
    store.shards.placements.push_back({shard, item.first, weight, "doc"});
    store.shards.load[shard] += weight;
  }
  for (const auto& page : store.pages) {
    const std::uint32_t shard = page->id % count;
    const std::uint64_t weight = page->bytes.size();
    store.shards.placements.push_back({shard, page->id, weight, "page"});
    store.shards.load[shard] += weight;
  }
  if (store.shards.placements.empty()) {
    return Result::fail(Status::invalid_state, "shard source empty");
  }
  store.shards.ready = true;
  store.shards.digest = digest_shards(store.shards);
  store.last_digest = store.shards.digest;
  store.events.push_back("shard-assign");
  return Result::success();
}

Result rebalance_shards(Store& store, std::uint32_t target_load) {
  if (!store.shards.ready || store.shards.load.empty()) {
    return Result::fail(Status::invalid_state, "shards not assigned");
  }
  if (target_load == 0) {
    return Result::fail(Status::limit, "shard target invalid");
  }
  std::sort(store.shards.placements.begin(), store.shards.placements.end(),
            [](const auto& left, const auto& right) {
              return left.weight > right.weight;
            });
  store.shards.load.assign(store.shards.count, 0);
  for (auto& entry : store.shards.placements) {
    const std::uint32_t shard = lightest_shard(store.shards.load);
    entry.shard = shard;
    store.shards.load[shard] += entry.weight;
    if (store.shards.load[shard] > target_load) {
      store.shards.load[shard] -= target_load / 16U;
    }
  }
  store.shards.digest = digest_shards(store.shards) ^ target_load;
  store.last_digest = store.shards.digest;
  store.events.push_back("shard-rebalance");
  return Result::success();
}

Result inspect_shards(Store& store) {
  if (!store.shards.ready) {
    return Result::fail(Status::invalid_state, "shards not assigned");
  }
  store.inspect.lines.push_back("shards=" + std::to_string(store.shards.count));
  for (std::size_t i = 0; i < store.shards.load.size(); ++i) {
    store.inspect.lines.push_back("load[" + std::to_string(i) + "]=" +
                                  std::to_string(store.shards.load[i]));
  }
  store.inspect.digest = hash_bytes(
      reinterpret_cast<const std::uint8_t*>(&store.shards.digest),
      sizeof(store.shards.digest), store.inspect.digest ^ 0x8d12e06ac7f51c9bULL);
  store.last_digest = store.inspect.digest;
  store.events.push_back("shard-inspect");
  return Result::success();
}

}  // namespace vesper
