#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::uint64_t digest_document(const Document& doc) {
  std::uint64_t digest = hash_bytes(
      reinterpret_cast<const std::uint8_t*>(doc.title), doc.title_size,
      0x13579bdf2468ace0ULL ^ doc.id);
  return hash_bytes(doc.body.data(), doc.body.size(), digest);
}

std::uint64_t digest_page(const Page& page) {
  return hash_bytes(page.bytes.data(), page.bytes.size(),
                    0x2468ace013579bdfULL ^ page.id ^ page.generation);
}

std::uint64_t digest_links(const LinkTable& links) {
  std::uint64_t digest = 0x9e3779b97f4a7c15ULL;
  for (std::uint32_t target : links.targets) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&target),
                        sizeof(target), digest);
  }
  if (links.selected.has_value()) digest ^= *links.selected;
  return digest;
}

std::uint64_t digest_nodes(const Store& store) {
  std::uint64_t digest = 0x94d049bb133111ebULL;
  for (const auto& node : store.nodes) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&node->id),
                        sizeof(node->id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&node->value),
                        sizeof(node->value), digest);
  }
  return digest ^ store.watches.size();
}

std::uint64_t digest_events(const Store& store) {
  std::uint64_t digest = 0xcbf29ce484222325ULL;
  for (const auto& event : store.events) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(event.data()),
                        event.size(), digest);
  }
  return digest;
}

std::uint64_t compute_kind_digest(Store& store, std::string_view kind,
                                  std::uint32_t object_id, bool* ok) {
  *ok = true;
  if (kind == "doc") {
    const auto it = store.documents.find(object_id);
    if (it == store.documents.end()) {
      *ok = false;
      return 0;
    }
    return digest_document(*it->second);
  }
  if (kind == "page") {
    for (const auto& page : store.pages) {
      if (page->id == object_id) return digest_page(*page);
    }
    *ok = false;
    return 0;
  }
  if (kind == "links") return digest_links(store.links) ^ object_id;
  if (kind == "nodes") return digest_nodes(store) ^ object_id;
  if (kind == "events") return digest_events(store) ^ object_id;
  *ok = false;
  return 0;
}

std::uint64_t summarize_digests(const DigestState& digests) {
  std::uint64_t digest = 0x6a09e667f3bcc909ULL;
  for (const auto& entry : digests.entries) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(entry.kind.data()),
                        entry.kind.size(), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.object_id),
                        sizeof(entry.object_id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.digest),
                        sizeof(entry.digest), digest);
  }
  if (digests.expected.has_value()) digest ^= *digests.expected;
  digest ^= digests.verified;
  digest ^= static_cast<std::uint64_t>(digests.matched);
  return digest;
}

}  // namespace

Result capture_digest(Store& store, std::string_view kind, std::uint32_t object_id) {
  bool ok = false;
  const std::uint64_t digest = compute_kind_digest(store, kind, object_id, &ok);
  if (!ok) {
    return Result::fail(Status::not_found, "digest source missing");
  }
  store.digests.entries.push_back(
      {std::string(kind), object_id, digest, ++store.epoch});
  if (store.digests.entries.size() > 128) {
    store.digests.entries.erase(store.digests.entries.begin(),
                                store.digests.entries.begin() + 32);
  }
  store.last_digest = summarize_digests(store.digests);
  store.events.push_back("digest-capture");
  return Result::success();
}

Result verify_digest(Store& store, std::uint64_t expected) {
  if (store.digests.entries.empty()) {
    return Result::fail(Status::invalid_state, "digest catalog empty");
  }
  store.digests.expected = expected;
  store.digests.verified = store.digests.entries.back().digest;
  store.digests.matched = (store.digests.verified == expected);
  store.last_digest = summarize_digests(store.digests);
  store.events.push_back(store.digests.matched ? "digest-match" : "digest-miss");
  return Result::success();
}

}  // namespace vesper
