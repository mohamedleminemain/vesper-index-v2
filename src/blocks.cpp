#include "vesper/engine.hpp"

#include <algorithm>

namespace vesper {
namespace {

BlockChunk pack_page(Page& page, std::uint32_t block_size) {
  BlockChunk chunk;
  chunk.page_id = page.id;
  chunk.ordinal = 0;
  chunk.flat = page.bytes;
  for (std::size_t i = 0; i < page.bytes.size();) {
    const std::uint8_t value = page.bytes[i];
    std::size_t run = 1;
    while (i + run < page.bytes.size() && page.bytes[i + run] == value &&
           run < block_size) {
      ++run;
    }
    chunk.runs.push_back({value, static_cast<std::uint32_t>(run)});
    i += run;
    ++chunk.ordinal;
  }
  chunk.digest = hash_bytes(page.bytes.data(), page.bytes.size(),
                            0x1bd11bdaa9fc1a22ULL ^ page.id);
  return chunk;
}

std::vector<std::uint8_t> unpack_chunk(const BlockChunk& chunk) {
  std::vector<std::uint8_t> out;
  for (const auto& run : chunk.runs) {
    out.insert(out.end(), run.length, run.value);
  }
  return out;
}

std::uint64_t digest_blocks(const BlockState& blocks) {
  std::uint64_t digest = 0xcbbb9d5dc1059ed8ULL ^ blocks.block_size;
  for (const auto& chunk : blocks.chunks) {
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&chunk.page_id),
                        sizeof(chunk.page_id), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&chunk.ordinal),
                        sizeof(chunk.ordinal), digest);
    digest = hash_bytes(reinterpret_cast<const std::uint8_t*>(&chunk.digest),
                        sizeof(chunk.digest), digest);
  }
  digest ^= blocks.packed_bytes;
  digest ^= blocks.unpacked_bytes << 5U;
  return digest;
}

}  // namespace

Result pack_blocks(Store& store, std::uint32_t block_size) {
  if (block_size == 0 || block_size > 255) {
    return Result::fail(Status::limit, "block size invalid");
  }
  store.blocks.chunks.clear();
  store.blocks.block_size = block_size;
  store.blocks.packed_bytes = 0;
  store.blocks.unpacked_bytes = 0;
  for (const auto& page : store.pages) {
    BlockChunk chunk = pack_page(*page, block_size);
    store.blocks.packed_bytes += chunk.runs.size() * sizeof(BlockRun);
    store.blocks.unpacked_bytes += chunk.flat.size();
    store.blocks.chunks.push_back(std::move(chunk));
  }
  if (store.blocks.chunks.empty()) {
    return Result::fail(Status::invalid_state, "block source empty");
  }
  store.blocks.ready = true;
  store.last_digest = digest_blocks(store.blocks);
  store.events.push_back("blocks-pack");
  return Result::success();
}

Result unpack_blocks(Store& store) {
  if (!store.blocks.ready) {
    return Result::fail(Status::invalid_state, "blocks not packed");
  }
  std::uint64_t digest = 0xd1310ba698dfb5acULL;
  for (const auto& chunk : store.blocks.chunks) {
    const auto data = unpack_chunk(chunk);
    if (data != chunk.flat) {
      return Result::fail(Status::invalid_state, "block roundtrip mismatch");
    }
    digest = hash_bytes(data.data(), data.size(), digest ^ chunk.page_id);
  }
  store.last_digest = digest_blocks(store.blocks) ^ digest;
  store.events.push_back("blocks-unpack");
  return Result::success();
}

}  // namespace vesper
