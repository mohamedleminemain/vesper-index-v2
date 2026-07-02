#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::uint64_t row_weight(const MergeCursorRow& row) {
  return (static_cast<std::uint64_t>(row.rank) << 32U) ^ row.id ^ row.weight;
}

std::uint64_t summarize_merge(const MergeState& merge) {
  std::uint64_t digest = 0xbb67ae8584caa73bULL;
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(merge.mode.data()),
                      merge.mode.size(), digest);
  for (const auto& row : merge.rows) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&row.rank),
                        sizeof(row.rank), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&row.id),
                        sizeof(row.id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(row.kind.data()),
                        row.kind.size(), digest);
  }
  return digest ^ merge.cursor;
}

void append_document_rows(Store& store, std::vector<MergeCursorRow>& rows) {
  for (const auto& item : store.documents) {
    rows.push_back({static_cast<std::uint32_t>(rows.size()), item.first, "doc",
                    item.second->title_size + item.second->body.size()});
  }
}

void append_page_rows(Store& store, std::vector<MergeCursorRow>& rows) {
  for (const auto& page : store.pages) {
    rows.push_back({static_cast<std::uint32_t>(rows.size()), page->id, "page",
                    page->bytes.size() + page->generation});
  }
}

void append_link_rows(Store& store, std::vector<MergeCursorRow>& rows) {
  for (std::uint32_t target : store.links.targets) {
    rows.push_back({static_cast<std::uint32_t>(rows.size()), target, "link",
                    target * 3ULL});
  }
}

}  // namespace

Result prepare_merge(Store& store, std::string_view mode) {
  if (mode.empty() || mode.size() > 16) {
    return Result::fail(Status::limit, "merge mode invalid");
  }
  store.merge.rows.clear();
  store.merge.mode.assign(mode);
  if (mode == "docs") {
    append_document_rows(store, store.merge.rows);
  } else if (mode == "pages") {
    append_page_rows(store, store.merge.rows);
  } else if (mode == "links") {
    append_link_rows(store, store.merge.rows);
  } else if (mode == "mixed") {
    append_document_rows(store, store.merge.rows);
    append_page_rows(store, store.merge.rows);
    append_link_rows(store, store.merge.rows);
  } else {
    return Result::fail(Status::unsupported, "unknown merge mode");
  }
  if (store.merge.rows.empty()) {
    return Result::fail(Status::invalid_state, "merge source empty");
  }
  std::sort(store.merge.rows.begin(), store.merge.rows.end(),
            [](const auto& left, const auto& right) {
              if (left.weight != right.weight) return left.weight > right.weight;
              if (left.kind != right.kind) return left.kind < right.kind;
              return left.id < right.id;
            });
  for (std::size_t i = 0; i < store.merge.rows.size(); ++i) {
    store.merge.rows[i].rank = static_cast<std::uint32_t>(i);
  }
  store.merge.cursor = 0;
  store.merge.prepared = true;
  store.merge.digest = summarize_merge(store.merge);
  store.last_digest = store.merge.digest;
  store.events.push_back("merge-prepare");
  return Result::success();
}

Result next_merge(Store& store, std::uint32_t steps) {
  if (!store.merge.prepared) {
    return Result::fail(Status::invalid_state, "merge not prepared");
  }
  if (steps == 0 || steps > 1024) {
    return Result::fail(Status::limit, "merge step invalid");
  }
  if (store.merge.cursor >= store.merge.rows.size()) {
    return Result::fail(Status::not_found, "merge exhausted");
  }
  const std::size_t end =
      std::min<std::size_t>(store.merge.rows.size(), store.merge.cursor + steps);
  std::uint64_t digest = 0x3c6ef372fe94f82bULL;
  for (std::size_t i = store.merge.cursor; i < end; ++i) {
    digest ^= row_weight(store.merge.rows[i]);
    digest = (digest << 5U) | (digest >> 59U);
  }
  store.merge.cursor = end;
  store.merge.digest = summarize_merge(store.merge) ^ digest;
  store.last_digest = store.merge.digest;
  store.events.push_back("merge-next");
  return Result::success();
}

}  // namespace vesper
