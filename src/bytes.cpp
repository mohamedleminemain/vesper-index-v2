#include "vesper/engine.hpp"

#include <array>
#include <charconv>
#include <limits>

namespace vesper {
namespace {

int hex_digit(char value) {
  if (value >= '0' && value <= '9') return value - '0';
  if (value >= 'a' && value <= 'f') return value - 'a' + 10;
  if (value >= 'A' && value <= 'F') return value - 'A' + 10;
  return -1;
}

}  // namespace

std::uint64_t hash_bytes(const std::uint8_t* data, std::size_t size,
                         std::uint64_t seed) {
  std::uint64_t hash = seed;
  for (std::size_t i = 0; i < size; ++i) {
    hash ^= data[i];
    hash *= 1099511628211ULL;
    hash ^= hash >> 29U;
  }
  return hash;
}

std::vector<std::uint8_t> decode_hex(std::string_view text, bool* ok) {
  std::vector<std::uint8_t> output;
  bool valid = text.size() % 2 == 0;
  if (valid) {
    output.reserve(text.size() / 2);
    for (std::size_t i = 0; i < text.size(); i += 2) {
      const int high = hex_digit(text[i]);
      const int low = hex_digit(text[i + 1]);
      if (high < 0 || low < 0) {
        valid = false;
        output.clear();
        break;
      }
      output.push_back(static_cast<std::uint8_t>((high << 4) | low));
    }
  }
  if (ok) *ok = valid;
  return output;
}

std::string encode_hex(const std::uint8_t* data, std::size_t size) {
  static constexpr std::array<char, 16> digits{
      '0', '1', '2', '3', '4', '5', '6', '7',
      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  std::string output;
  output.resize(size * 2);
  for (std::size_t i = 0; i < size; ++i) {
    output[i * 2] = digits[data[i] >> 4U];
    output[i * 2 + 1] = digits[data[i] & 0x0fU];
  }
  return output;
}

bool parse_u32(std::string_view text, std::uint32_t& value) {
  if (text.empty()) return false;
  std::uint64_t wide = 0;
  const auto result =
      std::from_chars(text.data(), text.data() + text.size(), wide, 10);
  if (result.ec != std::errc{} || result.ptr != text.data() + text.size() ||
      wide > std::numeric_limits<std::uint32_t>::max()) {
    return false;
  }
  value = static_cast<std::uint32_t>(wide);
  return true;
}

bool parse_u64(std::string_view text, std::uint64_t& value) {
  if (text.empty()) return false;
  const auto result =
      std::from_chars(text.data(), text.data() + text.size(), value, 10);
  return result.ec == std::errc{} &&
         result.ptr == text.data() + text.size();
}

}  // namespace vesper
