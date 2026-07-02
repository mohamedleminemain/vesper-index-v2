#include "vesper/engine.hpp"

#include <algorithm>
#include <cctype>
#include <string_view>

namespace vesper {
namespace {

bool is_word_char(unsigned char ch) {
  return std::isalnum(ch) || ch == '_' || ch == '-';
}

std::string canonicalize_token(std::string_view token) {
  std::string out;
  out.reserve(token.size());
  for (char ch : token) {
    const unsigned char value = static_cast<unsigned char>(ch);
    if (std::isalnum(value)) {
      out.push_back(static_cast<char>(std::tolower(value)));
    } else if (ch == '_' || ch == '-') {
      out.push_back('-');
    }
  }
  while (!out.empty() && out.front() == '-') out.erase(out.begin());
  while (!out.empty() && out.back() == '-') out.pop_back();
  return out;
}

void tokenize_text(std::string_view text, std::vector<std::string>& out) {
  std::size_t begin = 0;
  while (begin < text.size()) {
    while (begin < text.size() &&
           !is_word_char(static_cast<unsigned char>(text[begin]))) {
      ++begin;
    }
    std::size_t end = begin;
    while (end < text.size() &&
           is_word_char(static_cast<unsigned char>(text[end]))) {
      ++end;
    }
    if (end > begin) {
      std::string normalized = canonicalize_token(text.substr(begin, end - begin));
      if (!normalized.empty()) out.push_back(std::move(normalized));
    }
    begin = end + 1;
  }
}

void merge_token(NormalizerState& state, std::uint32_t source_id,
                 const std::string& raw, const std::string& normalized) {
  for (auto& token : state.tokens) {
    if (token.source_id == source_id && token.normalized == normalized) {
      ++token.count;
      return;
    }
  }
  state.tokens.push_back({raw, normalized, 1U, source_id});
}

std::uint64_t digest_normalizer(const NormalizerState& state) {
  std::uint64_t digest = 0x243f6a8885a308d3ULL;
  for (const auto& token : state.tokens) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(token.raw.data()),
                        token.raw.size(), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(
                            token.normalized.data()),
                        token.normalized.size(), digest);
    digest ^= static_cast<std::uint64_t>(token.count) << 12U;
    digest ^= static_cast<std::uint64_t>(token.source_id) << 3U;
  }
  for (const auto& line : state.emitted) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(line.data()),
                        line.size(), digest);
  }
  return digest;
}

}  // namespace

Result normalize_document(Store& store, std::uint32_t document_id) {
  const auto it = store.documents.find(document_id);
  if (it == store.documents.end()) {
    return Result::fail(Status::not_found, "normalize document missing");
  }
  store.normalizer.tokens.clear();
  store.normalizer.emitted.clear();

  std::vector<std::string> title_tokens;
  tokenize_text(std::string_view(it->second->title, it->second->title_size),
                title_tokens);
  for (const auto& token : title_tokens) {
    merge_token(store.normalizer, document_id, token, token);
  }

  std::string hex = encode_hex(it->second->body.data(), it->second->body.size());
  std::vector<std::string> body_tokens;
  tokenize_text(hex, body_tokens);
  for (const auto& token : body_tokens) {
    merge_token(store.normalizer, document_id, token, token);
  }

  std::sort(store.normalizer.tokens.begin(), store.normalizer.tokens.end(),
            [](const auto& left, const auto& right) {
              if (left.count != right.count) return left.count > right.count;
              if (left.normalized != right.normalized) {
                return left.normalized < right.normalized;
              }
              return left.source_id < right.source_id;
            });
  store.normalizer.ready = true;
  store.normalizer.digest = digest_normalizer(store.normalizer);
  store.last_digest = store.normalizer.digest;
  store.events.push_back("normalize-document");
  return Result::success();
}

Result emit_normalized_tokens(Store& store, std::uint32_t limit) {
  if (!store.normalizer.ready) {
    return Result::fail(Status::invalid_state, "normalizer not ready");
  }
  if (limit == 0 || limit > 256) {
    return Result::fail(Status::limit, "token emit limit invalid");
  }
  store.normalizer.emitted.clear();
  const std::size_t capped =
      std::min<std::size_t>(store.normalizer.tokens.size(), limit);
  for (std::size_t i = 0; i < capped; ++i) {
    const auto& token = store.normalizer.tokens[i];
    store.normalizer.emitted.push_back(token.normalized + ":" +
                                       std::to_string(token.count));
  }
  store.normalizer.digest = digest_normalizer(store.normalizer);
  store.last_digest = store.normalizer.digest;
  store.events.push_back("normalize-emit");
  return Result::success();
}

}  // namespace vesper
