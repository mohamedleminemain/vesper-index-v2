#include "vesper/engine.hpp"

namespace vesper {

Result emit_manifest(Store& store) {
  std::uint64_t digest = 0x6d616e6966657374ULL;
  for (const TitleView& view : store.title_views) {
    // Views are intentionally consumed here, far from their creation and update.
    for (std::size_t i = 0; i < view.size; ++i) {
      digest ^= static_cast<unsigned char>(view.data[i]);
      digest = (digest << 7U) | (digest >> 57U);
      digest *= 0x9e3779b185ebca87ULL;
    }
    digest ^= view.document_id;
  }
  for (const auto& document : store.documents) {
    digest = hash_bytes(document.second->body.data(),
                        document.second->body.size(), digest);
  }
  store.last_digest = digest;
  store.events.push_back("manifest-emitted");
  return Result::success();
}

}  // namespace vesper
