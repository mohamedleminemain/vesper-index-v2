#include "vesper/engine.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <string_view>

namespace vesper {
namespace {

std::string lower_ascii(std::string_view text) {
  std::string lowered;
  lowered.reserve(text.size());
  for (char ch : text) {
    lowered.push_back(static_cast<char>(
        std::tolower(static_cast<unsigned char>(ch))));
  }
  return lowered;
}

void collect_bloom_tokens(Store& store, std::vector<std::string>& tokens) {
  for (const auto& item : store.documents) {
    tokens.push_back(lower_ascii(std::string_view(item.second->title,
                                                 item.second->title_size)));
    if (!item.second->body.empty()) {
      tokens.push_back(encode_hex(item.second->body.data(),
                                  std::min<std::size_t>(item.second->body.size(),
                                                        12)));
    }
  }
  for (const auto& page : store.pages) {
    if (!page->bytes.empty()) {
      tokens.push_back(encode_hex(page->bytes.data(),
                                  std::min<std::size_t>(page->bytes.size(), 10)));
    }
  }
  for (const Field& field : store.schema.fields) {
    tokens.push_back(lower_ascii(field.name));
  }
  for (std::uint32_t link : store.links.targets) {
    tokens.push_back("link:" + std::to_string(link));
  }
}

std::uint32_t slot_for(std::string_view token, std::uint32_t round,
                       std::uint32_t slots, std::uint64_t seed) {
  const std::uint64_t mixed_seed =
      seed ^ (0x9e3779b97f4a7c15ULL + static_cast<std::uint64_t>(round) * 131U);
  const auto digest =
      hash_bytes(reinterpret_cast<const std::uint8_t*>(token.data()), token.size(),
                 mixed_seed);
  return static_cast<std::uint32_t>(digest % slots);
}

void set_bit(BloomFilterState& bloom, std::uint32_t bit) {
  const std::size_t word = bit / 64U;
  const std::uint32_t offset = bit % 64U;
  bloom.words[word] |= (1ULL << offset);
}

bool test_bit(const BloomFilterState& bloom, std::uint32_t bit) {
  const std::size_t word = bit / 64U;
  const std::uint32_t offset = bit % 64U;
  return (bloom.words[word] & (1ULL << offset)) != 0;
}

std::uint64_t digest_bloom(const BloomFilterState& bloom) {
  std::uint64_t digest = 0x1f83d9abfb41bd6bULL ^ bloom.hash_functions;
  for (std::uint64_t word : bloom.words) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&word),
                        sizeof(word), digest);
  }
  digest ^= bloom.inserted << 7U;
  return digest;
}

}  // namespace

Result build_bloom(Store& store, std::uint32_t word_count,
                   std::uint32_t hash_count) {
  if (word_count < 2 || word_count > 1024 || hash_count == 0 ||
      hash_count > 16) {
    return Result::fail(Status::limit, "bloom parameters invalid");
  }
  std::vector<std::string> tokens;
  collect_bloom_tokens(store, tokens);
  if (tokens.empty()) {
    return Result::fail(Status::invalid_state, "bloom source empty");
  }
  store.bloom.words.assign(word_count, 0);
  store.bloom.probes.clear();
  store.bloom.hash_functions = hash_count;
  store.bloom.inserted = 0;
  store.bloom.seed = store.last_digest ^ 0xa4093822299f31d0ULL;
  const std::uint32_t slots = word_count * 64U;
  for (const std::string& token : tokens) {
    std::array<std::uint32_t, 16> local{};
    for (std::uint32_t round = 0; round < hash_count; ++round) {
      const std::uint32_t slot = slot_for(token, round, slots, store.bloom.seed);
      local[round] = slot;
      set_bit(store.bloom, slot);
    }
    ++store.bloom.inserted;
    BloomProbe sample;
    sample.token = token;
    sample.maybe_present = true;
    for (std::uint32_t round = 0; round < hash_count; ++round) {
      sample.slots.push_back(local[round]);
    }
    if (store.bloom.probes.size() < 24) {
      store.bloom.probes.push_back(std::move(sample));
    }
  }
  store.bloom.ready = true;
  store.last_digest = digest_bloom(store.bloom);
  store.events.push_back("bloom-build");
  return Result::success();
}

Result probe_bloom(Store& store, std::string_view token) {
  if (!store.bloom.ready) {
    return Result::fail(Status::invalid_state, "bloom not ready");
  }
  if (token.empty() || token.size() > 128) {
    return Result::fail(Status::limit, "bloom token invalid");
  }
  const std::string normalized = lower_ascii(token);
  const std::uint32_t slots =
      static_cast<std::uint32_t>(store.bloom.words.size() * 64U);
  BloomProbe probe;
  probe.token = normalized;
  probe.maybe_present = true;
  for (std::uint32_t round = 0; round < store.bloom.hash_functions; ++round) {
    const std::uint32_t slot = slot_for(normalized, round, slots, store.bloom.seed);
    probe.slots.push_back(slot);
    if (!test_bit(store.bloom, slot)) {
      probe.maybe_present = false;
    }
  }
  store.bloom.probes.push_back(probe);
  if (store.bloom.probes.size() > 64) {
    store.bloom.probes.erase(store.bloom.probes.begin(),
                             store.bloom.probes.begin() + 16);
  }
  store.last_digest = digest_bloom(store.bloom) ^
                      hash_bytes(reinterpret_cast<const std::uint8_t*>(
                                     normalized.data()),
                                 normalized.size(), 0x94d049bb133111ebULL) ^
                      static_cast<std::uint64_t>(probe.maybe_present);
  store.events.push_back(probe.maybe_present ? "bloom-hit" : "bloom-miss");
  return Result::success();
}

}  // namespace vesper
