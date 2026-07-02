#include "vesper/engine.hpp"

namespace vesper {

Result configure_label(Store& store, std::uint32_t declared,
                       std::string text) {
  if (declared == 0 || declared > 255 || text.size() != declared) {
    return Result::fail(Status::malformed, "label length mismatch");
  }
  if (declared > 32) {
    return Result::fail(Status::limit, "label too wide");
  }
  store.label_plan.declared_size = declared;
  store.label_plan.text = std::move(text);
  store.events.push_back("label-configured");
  return Result::success();
}

}  // namespace vesper
