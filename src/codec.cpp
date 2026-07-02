#include "vesper/engine.hpp"

namespace vesper {

Result decode_vector(Store& store) {
  VectorPlan& plan = store.vector_plan;
  if (!plan.output || plan.source.empty()) {
    return Result::fail(Status::invalid_state, "vector not configured");
  }
  const std::size_t logical =
      static_cast<std::size_t>(plan.count) * plan.width;
  for (std::size_t i = 0; i < logical; ++i) {
    plan.output[i] =
        static_cast<std::uint8_t>(plan.source[i % plan.source.size()] ^
                                  static_cast<std::uint8_t>(i * 17U));
  }
  store.last_digest = hash_bytes(plan.output, plan.allocated);
  store.events.push_back("vector-decoded");
  return Result::success();
}

}  // namespace vesper
