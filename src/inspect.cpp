#include "vesper/engine.hpp"

#include <string_view>

namespace vesper {

Result inspect_store(Store& store, std::string_view view) {
  store.inspect.lines.clear();
  if (view == "catalog") {
    store.inspect.lines.push_back("documents=" + std::to_string(store.documents.size()));
    store.inspect.lines.push_back("pages=" + std::to_string(store.pages.size()));
    store.inspect.lines.push_back("nodes=" + std::to_string(store.nodes.size()));
    store.inspect.lines.push_back("events=" + std::to_string(store.events.size()));
  } else if (view == "query") {
    store.inspect.lines.push_back("bloom=" +
                                  std::to_string(store.bloom.words.size()));
    store.inspect.lines.push_back("skip-layers=" +
                                  std::to_string(store.skip.layers.size()));
    store.inspect.lines.push_back("plan-steps=" +
                                  std::to_string(store.plan.steps.size()));
    store.inspect.lines.push_back("metrics-samples=" +
                                  std::to_string(store.metrics.samples.size()));
  } else if (view == "storage") {
    store.inspect.lines.push_back("segments=" +
                                  std::to_string(store.segments.sealed.size()));
    store.inspect.lines.push_back("snapshots=" +
                                  std::to_string(store.snapshots.captured.size()));
    store.inspect.lines.push_back("cache-windows=" +
                                  std::to_string(store.cache.windows.size()));
    store.inspect.lines.push_back("block-chunks=" +
                                  std::to_string(store.blocks.chunks.size()));
  } else if (view == "security") {
    store.inspect.lines.push_back("roles=" + std::to_string(store.acl.roles.size()));
    store.inspect.lines.push_back("checks=" + std::to_string(store.acl.checks.size()));
    store.inspect.lines.push_back("timeline=" +
                                  std::to_string(store.timeline.events.size()));
  } else {
    return Result::fail(Status::unsupported, "unknown inspect view");
  }
  std::uint64_t digest = 0x4a7484aa6ea6e483ULL;
  for (const auto& line : store.inspect.lines) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(line.data()),
                        line.size(), digest);
  }
  store.inspect.digest = digest;
  store.last_digest = digest;
  store.events.push_back("inspect-store");
  return Result::success();
}

}  // namespace vesper
