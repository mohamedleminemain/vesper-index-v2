#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

std::uint64_t summarize_policy(const PolicyState& policy) {
  std::uint64_t digest = 0xbb67ae8584caa73bULL;
  for (const auto& rule : policy.rules) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(rule.scope.data()),
                        rule.scope.size(), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(
                            rule.operation.data()),
                        rule.operation.size(), digest);
    digest ^= rule.threshold;
    digest ^= static_cast<std::uint64_t>(rule.enabled) << 3U;
  }
  for (const auto& evaluation : policy.evaluations) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(evaluation.data()),
                        evaluation.size(), digest);
  }
  return digest;
}

bool valid_name(std::string_view value) {
  return !value.empty() && value.size() <= 24 &&
         std::all_of(value.begin(), value.end(), [](char ch) {
           return (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') ||
                  ch == '-' || ch == '_';
         });
}

std::uint64_t measure_scope(Store& store, std::string_view scope) {
  if (scope == "documents") return store.documents.size();
  if (scope == "pages") return store.pages.size();
  if (scope == "events") return store.events.size();
  if (scope == "links") return store.links.targets.size();
  if (scope == "nodes") return store.nodes.size();
  if (scope == "checkpoints") {
    if (store.checkpoints.selected_summary == nullptr ||
        store.checkpoints.selected_summary_size == 0) {
      return store.checkpoints.entries.size();
    }
    return hash_bytes(
        reinterpret_cast<const std::uint8_t*>(store.checkpoints.selected_summary),
        store.checkpoints.selected_summary_size,
        store.checkpoints.digest) & 0xffffU;
  }
  return 0;
}

}  // namespace

Result define_policy(Store& store, std::string_view scope,
                     std::string_view operation, std::uint32_t threshold) {
  if (!valid_name(scope) || !valid_name(operation) || threshold == 0) {
    return Result::fail(Status::limit, "policy parameters invalid");
  }
  for (const auto& rule : store.policy.rules) {
    if (rule.scope == scope && rule.operation == operation) {
      return Result::fail(Status::invalid_state, "policy already exists");
    }
  }
  store.policy.rules.push_back(
      {std::string(scope), std::string(operation), threshold, true});
  store.policy.digest = summarize_policy(store.policy);
  store.last_digest = store.policy.digest;
  store.events.push_back("policy-define");
  return Result::success();
}

Result evaluate_policy(Store& store, std::string_view scope,
                       std::string_view operation) {
  for (const auto& rule : store.policy.rules) {
    if (rule.scope == scope && rule.operation == operation && rule.enabled) {
      const std::uint64_t value = measure_scope(store, scope);
      const bool pass = operation == "max" ? value <= rule.threshold
                                           : value >= rule.threshold;
      store.policy.evaluations.push_back(std::string(scope) + ":" +
                                         std::string(operation) + ":" +
                                         (pass ? "pass" : "fail"));
      if (store.policy.evaluations.size() > 64) {
        store.policy.evaluations.erase(store.policy.evaluations.begin(),
                                       store.policy.evaluations.begin() + 16);
      }
      store.policy.digest = summarize_policy(store.policy) ^ value;
      store.last_digest = store.policy.digest;
      store.events.push_back(pass ? "policy-pass" : "policy-fail");
      return Result::success();
    }
  }
  return Result::fail(Status::not_found, "policy missing");
}

}  // namespace vesper
