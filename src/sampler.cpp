#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

void sample_documents(Store& store, std::vector<SampleRow>& rows) {
  for (const auto& item : store.documents) {
    rows.push_back({static_cast<std::uint32_t>(rows.size()),
                    item.second->title_size + item.second->body.size(),
                    "doc:" + std::to_string(item.first)});
  }
}

void sample_pages(Store& store, std::vector<SampleRow>& rows) {
  for (const auto& page : store.pages) {
    rows.push_back({static_cast<std::uint32_t>(rows.size()),
                    page->bytes.size() + page->generation,
                    "page:" + std::to_string(page->id)});
  }
}

void sample_nodes(Store& store, std::vector<SampleRow>& rows) {
  for (const auto& node : store.nodes) {
    rows.push_back({static_cast<std::uint32_t>(rows.size()), node->value,
                    "node:" + std::to_string(node->id)});
  }
}

std::uint64_t summarize_sample(const SampleState& sample) {
  std::uint64_t digest = 0x1f83d9abfb41bd6bULL;
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(sample.scope.data()),
                      sample.scope.size(), digest);
  for (const auto& row : sample.rows) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&row.value),
                        sizeof(row.value), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(row.label.data()),
                        row.label.size(), digest);
  }
  for (std::uint64_t fold : sample.folds) {
    digest ^= fold;
  }
  return digest ^ sample.limit;
}

}  // namespace

Result sample_scope(Store& store, std::string_view scope, std::uint32_t limit) {
  if (scope.empty() || scope.size() > 16 || limit == 0 || limit > 512) {
    return Result::fail(Status::limit, "sample parameters invalid");
  }
  store.sample.scope.assign(scope);
  store.sample.limit = limit;
  store.sample.rows.clear();
  store.sample.folds.clear();
  if (scope == "docs") {
    sample_documents(store, store.sample.rows);
  } else if (scope == "pages") {
    sample_pages(store, store.sample.rows);
  } else if (scope == "nodes") {
    sample_nodes(store, store.sample.rows);
  } else if (scope == "all") {
    sample_documents(store, store.sample.rows);
    sample_pages(store, store.sample.rows);
    sample_nodes(store, store.sample.rows);
  } else {
    return Result::fail(Status::unsupported, "unknown sample scope");
  }
  if (store.sample.rows.empty()) {
    return Result::fail(Status::invalid_state, "sample source empty");
  }
  std::sort(store.sample.rows.begin(), store.sample.rows.end(),
            [](const auto& left, const auto& right) {
              if (left.value != right.value) return left.value > right.value;
              return left.label < right.label;
            });
  if (store.sample.rows.size() > limit) {
    store.sample.rows.resize(limit);
  }
  store.sample.digest = summarize_sample(store.sample);
  store.last_digest = store.sample.digest;
  store.events.push_back("sample-scope");
  return Result::success();
}

Result fold_sample(Store& store) {
  if (store.sample.rows.empty()) {
    return Result::fail(Status::invalid_state, "sample not prepared");
  }
  store.sample.folds.clear();
  std::uint64_t fold = 0x5be0cd19137e2179ULL;
  for (const auto& row : store.sample.rows) {
    fold ^= row.value + row.ordinal;
    fold = (fold << 7U) | (fold >> 57U);
    store.sample.folds.push_back(fold);
  }
  store.sample.digest = summarize_sample(store.sample);
  store.last_digest = store.sample.digest;
  store.events.push_back("sample-fold");
  return Result::success();
}

}  // namespace vesper
