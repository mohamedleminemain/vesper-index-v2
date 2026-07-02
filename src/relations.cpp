#include "vesper/engine.hpp"

namespace vesper {
namespace {

std::uint64_t summarize_relations(const RelationState& relation) {
  std::uint64_t digest = 0x1f83d9abfb41bd6bULL;
  for (const auto& edge : relation.edges) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&edge.from),
                        sizeof(edge.from), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&edge.to),
                        sizeof(edge.to), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(edge.kind.data()),
                        edge.kind.size(), digest);
  }
  for (std::uint32_t value : relation.walk) {
    digest ^= value;
  }
  return digest;
}

void append_link_edges(Store& store, std::vector<RelationEdge>& edges) {
  for (std::uint32_t target : store.links.targets) {
    edges.push_back({0, target, "link", target * 5ULL});
  }
}

void append_doc_edges(Store& store, std::vector<RelationEdge>& edges) {
  for (const auto& item : store.documents) {
    edges.push_back({item.first, static_cast<std::uint32_t>(item.second->title_size),
                     "doc", item.second->body.size()});
  }
}

void append_node_edges(Store& store, std::vector<RelationEdge>& edges) {
  for (const auto& node : store.nodes) {
    edges.push_back({node->id, static_cast<std::uint32_t>(node->value & 0xffffffffU),
                     "node", node->value});
  }
}

}  // namespace

Result build_relations(Store& store, std::string_view mode) {
  if (mode.empty() || mode.size() > 16) {
    return Result::fail(Status::limit, "relation mode invalid");
  }
  store.relation.edges.clear();
  store.relation.walk.clear();
  if (mode == "links") {
    append_link_edges(store, store.relation.edges);
  } else if (mode == "docs") {
    append_doc_edges(store, store.relation.edges);
  } else if (mode == "nodes") {
    append_node_edges(store, store.relation.edges);
  } else if (mode == "mixed") {
    append_doc_edges(store, store.relation.edges);
    append_link_edges(store, store.relation.edges);
    append_node_edges(store, store.relation.edges);
  } else {
    return Result::fail(Status::unsupported, "unknown relation mode");
  }
  if (store.relation.edges.empty()) {
    return Result::fail(Status::invalid_state, "relation source empty");
  }
  store.relation.prepared = true;
  store.relation.digest = summarize_relations(store.relation);
  store.last_digest = store.relation.digest;
  store.events.push_back("relation-build");
  return Result::success();
}

Result walk_relations(Store& store, std::uint32_t start, std::uint32_t steps) {
  if (!store.relation.prepared) {
    return Result::fail(Status::invalid_state, "relations not prepared");
  }
  if (steps == 0 || steps > 1024) {
    return Result::fail(Status::limit, "relation steps invalid");
  }
  store.relation.walk.clear();
  std::uint32_t current = start;
  for (std::uint32_t i = 0; i < steps; ++i) {
    bool advanced = false;
    for (const auto& edge : store.relation.edges) {
      if (edge.from == current || edge.to == current) {
        current = edge.to;
        store.relation.walk.push_back(current);
        advanced = true;
        break;
      }
    }
    if (!advanced) break;
  }
  if (store.relation.walk.empty()) {
    return Result::fail(Status::not_found, "relation walk empty");
  }
  store.relation.digest = summarize_relations(store.relation);
  store.last_digest = store.relation.digest;
  store.events.push_back("relation-walk");
  return Result::success();
}

}  // namespace vesper
