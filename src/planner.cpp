#include "vesper/engine.hpp"

#include <algorithm>
#include <string_view>

namespace vesper {
namespace {

std::uint64_t estimate_document_cost(const Store& store) {
  std::uint64_t total = 0;
  for (const auto& item : store.documents) {
    total += item.second->title_size * 3U + item.second->body.size();
  }
  return total;
}

std::uint64_t estimate_page_cost(const Store& store) {
  std::uint64_t total = 0;
  for (const auto& page : store.pages) {
    total += page->bytes.size() + (page->generation % 31U);
  }
  return total;
}

std::uint64_t estimate_graph_cost(const Store& store) {
  std::uint64_t total = store.links.targets.size() * 11U;
  total += store.nodes.size() * 13U;
  total += store.watches.size() * 7U;
  return total;
}

PlanStep make_step(std::string name, std::uint64_t cost, std::uint32_t fanout,
                   std::uint32_t cardinality, bool blocking) {
  PlanStep step;
  step.name = std::move(name);
  step.estimated_cost = cost;
  step.fanout = fanout;
  step.cardinality = cardinality;
  step.blocking = blocking;
  return step;
}

std::uint64_t digest_plan(const QueryPlanState& plan) {
  std::uint64_t digest = 0x510e527fade682d1ULL;
  digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(plan.mode.data()),
                      plan.mode.size(), digest);
  for (const auto& step : plan.steps) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(step.name.data()),
                        step.name.size(), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&step.estimated_cost),
                        sizeof(step.estimated_cost), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&step.fanout),
                        sizeof(step.fanout), digest);
  }
  digest ^= plan.total_cost;
  digest ^= static_cast<std::uint64_t>(plan.limit) << 7U;
  digest ^= plan.result_digest << 3U;
  return digest;
}

}  // namespace

Result build_query_plan(Store& store, std::string_view mode,
                        std::uint32_t limit) {
  if (mode.empty() || mode.size() > 32 || limit == 0 || limit > 4096) {
    return Result::fail(Status::limit, "plan parameters invalid");
  }
  store.plan.mode.assign(mode);
  store.plan.steps.clear();
  store.plan.total_cost = 0;
  store.plan.limit = limit;
  store.plan.result_digest = 0;

  const std::uint64_t doc_cost = estimate_document_cost(store);
  const std::uint64_t page_cost = estimate_page_cost(store);
  const std::uint64_t graph_cost = estimate_graph_cost(store);
  const std::uint32_t doc_card = static_cast<std::uint32_t>(store.documents.size());
  const std::uint32_t page_card = static_cast<std::uint32_t>(store.pages.size());
  const std::uint32_t graph_card = static_cast<std::uint32_t>(
      store.links.targets.size() + store.nodes.size());

  if (mode == "docs" || mode == "mixed") {
    store.plan.steps.push_back(
        make_step("doc-scan", doc_cost + 3U, 4U, doc_card, false));
  }
  if (mode == "pages" || mode == "mixed") {
    store.plan.steps.push_back(
        make_step("page-scan", page_cost + 5U, 8U, page_card, false));
  }
  if (mode == "graph" || mode == "mixed") {
    store.plan.steps.push_back(
        make_step("graph-expand", graph_cost + 9U, 3U, graph_card, true));
  }
  if (store.bloom.ready) {
    store.plan.steps.push_back(make_step(
        "bloom-filter",
        static_cast<std::uint64_t>(store.bloom.hash_functions) * 17U,
        1U, doc_card + page_card, false));
  }
  if (store.skip.built) {
    store.plan.steps.push_back(make_step(
        "skip-seek",
        static_cast<std::uint64_t>(store.skip.layers.size()) * 19U,
        1U, page_card, false));
  }
  if (store.plan.steps.empty()) {
    return Result::fail(Status::invalid_state, "plan source empty");
  }

  for (const auto& step : store.plan.steps) {
    store.plan.total_cost += step.estimated_cost +
                             static_cast<std::uint64_t>(step.fanout) * step.cardinality;
  }
  store.plan.ready = true;
  store.last_digest = digest_plan(store.plan);
  store.events.push_back("plan-build");
  return Result::success();
}

Result evaluate_query_plan(Store& store) {
  if (!store.plan.ready || store.plan.steps.empty()) {
    return Result::fail(Status::invalid_state, "plan not ready");
  }
  std::uint64_t digest = 0x9b05688c2b3e6c1fULL;
  std::uint64_t budget = store.plan.limit;
  for (const auto& step : store.plan.steps) {
    const std::uint64_t consumed =
        std::min<std::uint64_t>(budget, step.cardinality + step.fanout);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(step.name.data()),
                        step.name.size(), digest ^ consumed);
    digest ^= step.estimated_cost + consumed * (step.blocking ? 5U : 2U);
    if (budget > consumed) {
      budget -= consumed;
    } else {
      budget = 0;
    }
  }
  store.plan.result_digest = digest ^ budget;
  store.last_digest = digest_plan(store.plan);
  store.events.push_back("plan-eval");
  return Result::success();
}

}  // namespace vesper
