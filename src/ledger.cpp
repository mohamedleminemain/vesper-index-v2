#include "vesper/engine.hpp"

namespace vesper {
namespace {

std::uint64_t summarize_ledger(const LedgerState& ledger) {
  std::uint64_t digest = 0x243f6a8885a308d3ULL;
  for (const auto& entry : ledger.entries) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(entry.tag.data()),
                        entry.tag.size(), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.value),
                        sizeof(entry.value), digest);
  }
  return digest ^ ledger.folded;
}

void append_common_entries(Store& store, std::vector<LedgerEntry>& entries) {
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "documents",
                     store.documents.size()});
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "pages",
                     store.pages.size()});
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "events",
                     store.events.size()});
}

void append_query_entries(Store& store, std::vector<LedgerEntry>& entries) {
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "links",
                     store.links.targets.size()});
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "nodes",
                     store.nodes.size()});
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "watches",
                     store.watches.size()});
}

void append_codec_entries(Store& store, std::vector<LedgerEntry>& entries) {
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "vector-width",
                     store.vector_plan.width});
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "label-size",
                     store.label_plan.declared_size});
  entries.push_back({static_cast<std::uint32_t>(entries.size()), "palette",
                     store.palette_plan.selected});
}

}  // namespace

Result build_ledger(Store& store, std::string_view mode) {
  if (mode.empty() || mode.size() > 16) {
    return Result::fail(Status::limit, "ledger mode invalid");
  }
  store.ledger.entries.clear();
  append_common_entries(store, store.ledger.entries);
  if (mode == "archive") {
    store.ledger.entries.push_back({static_cast<std::uint32_t>(store.ledger.entries.size()),
                                    "snapshots", store.snapshots.captured.size()});
    store.ledger.entries.push_back({static_cast<std::uint32_t>(store.ledger.entries.size()),
                                    "segments", store.segments.sealed.size()});
  } else if (mode == "query") {
    append_query_entries(store, store.ledger.entries);
  } else if (mode == "codec") {
    append_codec_entries(store, store.ledger.entries);
  } else if (mode == "all") {
    append_query_entries(store, store.ledger.entries);
    append_codec_entries(store, store.ledger.entries);
  } else {
    return Result::fail(Status::unsupported, "unknown ledger mode");
  }
  store.ledger.folded = 0;
  store.ledger.digest = summarize_ledger(store.ledger);
  store.last_digest = store.ledger.digest;
  store.events.push_back("ledger-build");
  return Result::success();
}

Result fold_ledger(Store& store) {
  if (store.ledger.entries.empty()) {
    return Result::fail(Status::invalid_state, "ledger empty");
  }
  std::uint64_t folded = 0x13198a2e03707344ULL;
  for (const auto& entry : store.ledger.entries) {
    folded ^= entry.value + entry.ordinal;
    folded = (folded << 9U) | (folded >> 55U);
  }
  store.ledger.folded = folded;
  store.ledger.digest = summarize_ledger(store.ledger);
  store.last_digest = store.ledger.digest;
  store.events.push_back("ledger-fold");
  return Result::success();
}

}  // namespace vesper
