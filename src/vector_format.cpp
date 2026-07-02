#include "vesper/engine.hpp"

namespace vesper {

Result configure_vector(Store& store, std::uint32_t count,
                        std::uint32_t width,
                        std::vector<std::uint8_t> source) {
  if (count == 0 || count > 100000 || width == 0 || width > 32 ||
      source.empty()) {
    return Result::fail(Status::limit, "vector limits exceeded");
  }
  store.vector_plan.reset();
  store.vector_plan.count = count;
  store.vector_plan.width = width;
  store.vector_plan.source = std::move(source);
  const std::size_t logical =
      static_cast<std::size_t>(count) * static_cast<std::size_t>(width);
  if (logical > (1U << 20U)) {
    return Result::fail(Status::limit, "vector storage limit exceeded");
  }
  const std::size_t storage = logical;
  store.vector_plan.output = new std::uint8_t[storage];
  store.vector_plan.allocated = storage;
  store.events.push_back("vector-configured");
  return Result::success();
}

}  // namespace vesper
