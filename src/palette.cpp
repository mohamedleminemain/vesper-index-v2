#include "vesper/engine.hpp"

#include <array>

namespace vesper {
namespace {

std::array<std::uint32_t, 16> kPalette{
    0x101820U, 0x182430U, 0x203040U, 0x284050U,
    0x305060U, 0x386070U, 0x407080U, 0x488090U,
    0x5090a0U, 0x58a0b0U, 0x60b0c0U, 0x68c0d0U,
    0x70d0e0U, 0x78e0f0U, 0x80f0ffU, 0xf0f8ffU};

}  // namespace

Result emit_palette_color(Store& store) {
  const std::uint32_t color = kPalette[store.palette_plan.selected];
  store.last_digest =
      (static_cast<std::uint64_t>(color) << 32U) | (color ^ 0x5a5a5aU);
  store.events.push_back("palette-emitted");
  return Result::success();
}

}  // namespace vesper
