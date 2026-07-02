#include "vesper/engine.hpp"

#include <algorithm>
#include <string_view>

namespace vesper {
namespace {

void append_document_samples(const Store& store, std::vector<std::uint64_t>& out) {
  for (const auto& item : store.documents) {
    out.push_back(item.second->title_size);
    out.push_back(item.second->body.size());
  }
}

void append_page_samples(const Store& store, std::vector<std::uint64_t>& out) {
  for (const auto& page : store.pages) {
    out.push_back(page->bytes.size());
    out.push_back(page->generation % 4096U);
  }
}

void append_graph_samples(const Store& store, std::vector<std::uint64_t>& out) {
  for (const auto& node : store.nodes) {
    out.push_back(node->value & 0xffffU);
  }
  for (std::uint32_t link : store.links.targets) {
    out.push_back(link);
  }
  out.push_back(store.watches.size());
}

std::vector<MetricBucket> build_buckets(const std::vector<std::uint64_t>& samples) {
  std::vector<MetricBucket> buckets;
  if (samples.empty()) return buckets;
  const std::uint64_t high = *std::max_element(samples.begin(), samples.end());
  const std::uint64_t width = std::max<std::uint64_t>(1, (high / 8U) + 1U);
  for (std::uint64_t lower = 0; lower <= high + width; lower += width) {
    buckets.push_back({lower, lower + width - 1, 0, 0});
    if (buckets.size() >= 16) break;
  }
  for (std::uint64_t sample : samples) {
    for (auto& bucket : buckets) {
      if (sample >= bucket.lower && sample <= bucket.upper) {
        ++bucket.count;
        bucket.weight += sample;
        break;
      }
    }
  }
  return buckets;
}

std::uint64_t digest_metrics(const MetricsState& metrics) {
  std::uint64_t digest = 0x5be0cd19137e2179ULL;
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(metrics.scope.data()),
                      metrics.scope.size(), digest);
  for (const auto& bucket : metrics.buckets) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&bucket.lower),
                        sizeof(bucket.lower), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&bucket.upper),
                        sizeof(bucket.upper), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&bucket.count),
                        sizeof(bucket.count), digest);
  }
  digest ^= metrics.reduced;
  digest ^= metrics.minimum << 1U;
  digest ^= metrics.maximum << 2U;
  return digest;
}

}  // namespace

Result collect_metrics(Store& store, std::string_view scope) {
  if (scope.empty() || scope.size() > 32) {
    return Result::fail(Status::limit, "metrics scope invalid");
  }
  store.metrics.scope.assign(scope);
  store.metrics.samples.clear();
  if (scope == "docs") {
    append_document_samples(store, store.metrics.samples);
  } else if (scope == "pages") {
    append_page_samples(store, store.metrics.samples);
  } else if (scope == "graph") {
    append_graph_samples(store, store.metrics.samples);
  } else if (scope == "all") {
    append_document_samples(store, store.metrics.samples);
    append_page_samples(store, store.metrics.samples);
    append_graph_samples(store, store.metrics.samples);
  } else {
    return Result::fail(Status::unsupported, "unknown metrics scope");
  }
  if (store.metrics.samples.empty()) {
    return Result::fail(Status::invalid_state, "metrics source empty");
  }
  std::sort(store.metrics.samples.begin(), store.metrics.samples.end());
  store.metrics.minimum = store.metrics.samples.front();
  store.metrics.maximum = store.metrics.samples.back();
  std::uint64_t sum = 0;
  for (std::uint64_t sample : store.metrics.samples) sum += sample;
  store.metrics.average = sum / store.metrics.samples.size();
  store.metrics.buckets = build_buckets(store.metrics.samples);
  store.metrics.reduced = 0;
  store.metrics.collected = true;
  store.last_digest = digest_metrics(store.metrics);
  store.events.push_back("metrics-collect");
  return Result::success();
}

Result reduce_metrics(Store& store) {
  if (!store.metrics.collected) {
    return Result::fail(Status::invalid_state, "metrics not collected");
  }
  std::uint64_t reduced = 0;
  for (const auto& bucket : store.metrics.buckets) {
    reduced ^= (bucket.weight + 1U) * (bucket.count + 3U);
    reduced = (reduced << 7U) | (reduced >> 57U);
  }
  reduced ^= store.metrics.average;
  reduced ^= store.metrics.maximum << 11U;
  store.metrics.reduced = reduced;
  store.last_digest = digest_metrics(store.metrics);
  store.events.push_back("metrics-reduce");
  return Result::success();
}

}  // namespace vesper
