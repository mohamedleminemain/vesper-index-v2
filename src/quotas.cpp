#include "vesper/engine.hpp"

namespace vesper {
namespace {

std::uint64_t summarize_quota(const QuotaState& quota) {
  std::uint64_t digest = 0x510e527fade682d1ULL;
  for (const auto& bucket : quota.buckets) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(bucket.name.data()),
                        bucket.name.size(), digest);
    digest ^= bucket.used;
    digest ^= bucket.limit << 5U;
    digest ^= static_cast<std::uint64_t>(bucket.exceeded) << 11U;
  }
  return digest;
}

}  // namespace

Result prepare_quota(Store& store, std::uint32_t scale) {
  if (scale == 0 || scale > 4096) {
    return Result::fail(Status::limit, "quota scale invalid");
  }
  store.quota.buckets.clear();
  store.quota.buckets.push_back(
      {"documents", store.documents.size(), scale * 2ULL, false});
  store.quota.buckets.push_back(
      {"pages", store.pages.size(), scale * 2ULL, false});
  store.quota.buckets.push_back(
      {"events", store.events.size(), scale * 8ULL, false});
  store.quota.buckets.push_back(
      {"links", store.links.targets.size(), scale * 4ULL, false});
  store.quota.buckets.push_back(
      {"nodes", store.nodes.size(), scale * 4ULL, false});
  for (auto& bucket : store.quota.buckets) {
    bucket.exceeded = bucket.used > bucket.limit;
  }
  store.quota.prepared = true;
  store.quota.digest = summarize_quota(store.quota);
  store.last_digest = store.quota.digest;
  store.events.push_back("quota-prepare");
  return Result::success();
}

Result enforce_quota(Store& store, std::string_view name) {
  if (!store.quota.prepared) {
    return Result::fail(Status::invalid_state, "quota not prepared");
  }
  for (auto& bucket : store.quota.buckets) {
    if (bucket.name == name) {
      bucket.exceeded = bucket.used > bucket.limit;
      store.quota.digest = summarize_quota(store.quota);
      store.last_digest = store.quota.digest;
      store.events.push_back(bucket.exceeded ? "quota-fail" : "quota-pass");
      return Result::success();
    }
  }
  return Result::fail(Status::not_found, "quota bucket missing");
}

}  // namespace vesper
