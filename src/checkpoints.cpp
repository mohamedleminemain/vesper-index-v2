#include "vesper/engine.hpp"

namespace vesper {
namespace {

std::uint64_t summarize_checkpoints(const CheckpointState& checkpoints) {
  std::uint64_t digest = 0x6a09e667f3bcc909ULL;
  for (const auto& entry : checkpoints.entries) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.id),
                        sizeof(entry.id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.epoch),
                        sizeof(entry.epoch), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&entry.digest),
                        sizeof(entry.digest), digest);
  }
  if (checkpoints.selected.has_value()) digest ^= *checkpoints.selected;
  return digest;
}

CheckpointEntry* find_checkpoint(Store& store, std::uint32_t id) {
  for (auto& entry : store.checkpoints.entries) {
    if (entry.id == id) return &entry;
  }
  return nullptr;
}

}  // namespace

Result capture_checkpoint(Store& store, std::uint32_t id) {
  if (id == 0) {
    return Result::fail(Status::limit, "checkpoint id invalid");
  }
  if (find_checkpoint(store, id) != nullptr) {
    return Result::fail(Status::invalid_state, "checkpoint already exists");
  }
  CheckpointEntry entry;
  entry.id = id;
  entry.epoch = store.epoch;
  entry.digest = store.last_digest ^ (static_cast<std::uint64_t>(id) << 17U);
  entry.documents = static_cast<std::uint32_t>(store.documents.size());
  entry.pages = static_cast<std::uint32_t>(store.pages.size());
  entry.events = static_cast<std::uint32_t>(store.events.size());
  store.checkpoints.entries.push_back(entry);
  store.checkpoints.selected = id;
  store.checkpoints.selected_summary = nullptr;
  store.checkpoints.selected_summary_size = 0;
  store.checkpoints.digest = summarize_checkpoints(store.checkpoints);
  store.last_digest = store.checkpoints.digest;
  store.events.push_back("checkpoint-capture");
  return Result::success();
}

Result inspect_checkpoint(Store& store, std::uint32_t id) {
  CheckpointEntry* entry = find_checkpoint(store, id);
  if (entry == nullptr) {
    return Result::fail(Status::not_found, "checkpoint missing");
  }
  store.checkpoints.selected = id;
  store.inspect.lines.clear();
  store.inspect.lines.push_back("checkpoint.id=" + std::to_string(entry->id));
  store.inspect.lines.push_back("checkpoint.docs=" +
                                std::to_string(entry->documents));
  store.inspect.lines.push_back("checkpoint.pages=" +
                                std::to_string(entry->pages));
  store.inspect.lines.push_back("checkpoint.events=" +
                                std::to_string(entry->events));
  store.inspect.lines.push_back("checkpoint.summary=" +
                                std::to_string(entry->id) + ":" +
                                std::to_string(entry->epoch) + ":" +
                                std::to_string(entry->digest) + ":" +
                                std::to_string(entry->documents) + ":" +
                                std::to_string(entry->pages) + ":" +
                                std::to_string(entry->events) +
                                ":retained-state-summary");
  store.checkpoints.selected_summary = store.inspect.lines.back().data();
  store.checkpoints.selected_summary_size = store.inspect.lines.back().size();
  store.checkpoints.digest = summarize_checkpoints(store.checkpoints);
  store.last_digest = store.checkpoints.digest ^ entry->digest;
  store.events.push_back("checkpoint-inspect");
  return Result::success();
}

}  // namespace vesper
