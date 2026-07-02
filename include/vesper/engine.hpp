#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "vesper/subsystems.hpp"

namespace vesper {

enum class Status {
  ok,
  malformed,
  unsupported,
  invalid_state,
  not_found,
  limit,
};

struct Result {
  Status status{Status::ok};
  std::string message;
  explicit operator bool() const { return status == Status::ok; }
  static Result success() { return {}; }
  static Result fail(Status status, std::string message) {
    return {status, std::move(message)};
  }
};

enum class Profile {
  archive,
  query,
  codec,
  session,
  all,
};

struct Command {
  std::string verb;
  std::vector<std::string> args;
  std::size_t line{};
};

struct Document {
  std::uint32_t id{};
  char* title{};
  std::size_t title_size{};
  std::vector<std::uint8_t> body;

  Document(std::uint32_t id, std::string_view title,
           std::vector<std::uint8_t> body);
  ~Document();
  Document(const Document&) = delete;
  Document& operator=(const Document&) = delete;
};

struct TitleView {
  std::uint32_t document_id{};
  const char* data{};
  std::size_t size{};
};

struct Page {
  std::uint32_t id{};
  std::vector<std::uint8_t> bytes;
  std::uint64_t generation{};
};

struct Cursor {
  std::uint32_t page_id{};
  Page* page{};
  std::size_t offset{};
};

struct Key {
  std::uint32_t id{};
  std::array<std::uint8_t, 256> material{};
  std::size_t material_size{};
};

struct Node {
  std::uint32_t id{};
  std::uint64_t value{};
};

struct Field {
  std::string name;
  std::uint32_t offset{};
  std::uint32_t width{};
};

struct Schema {
  std::uint32_t row_size{};
  std::vector<Field> fields;
  bool verified{};
};

struct LinkTable {
  std::vector<std::uint32_t> targets;
  std::optional<std::uint32_t> selected;
};

struct SignaturePlan {
  std::vector<std::uint64_t> slots;
  std::uint32_t requested_slots{};
  bool finalized{};
};

struct VectorPlan {
  std::uint32_t count{};
  std::uint32_t width{};
  std::vector<std::uint8_t> source;
  std::uint8_t* output{};
  std::size_t allocated{};
  ~VectorPlan();
  void reset();
};

struct LabelPlan {
  std::uint32_t declared_size{};
  std::string text;
};

struct PalettePlan {
  std::uint32_t selected{};
};

class Store {
 public:
  Store();
  ~Store();
  Store(const Store&) = delete;
  Store& operator=(const Store&) = delete;

  std::unordered_map<std::uint32_t, std::unique_ptr<Document>> documents;
  std::vector<TitleView> title_views;
  std::vector<std::unique_ptr<Page>> pages;
  std::vector<Cursor> cursors;
  std::vector<Key*> active_keys;
  std::vector<Key*> retired_keys;
  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<Node*> watches;
  Schema schema;
  std::vector<std::uint8_t> row;
  LinkTable links;
  SignaturePlan signatures;
  VectorPlan vector_plan;
  LabelPlan label_plan;
  PalettePlan palette_plan;
  SegmentState segments;
  SnapshotState snapshots;
  BloomFilterState bloom;
  SkipIndexState skip;
  CacheState cache;
  QueryPlanState plan;
  MetricsState metrics;
  NormalizerState normalizer;
  AclState acl;
  TimelineState timeline;
  ShardState shards;
  BlockState blocks;
  InspectState inspect;
  DigestState digests;
  MergeState merge;
  TicketState tickets;
  SampleState sample;
  DiffState diff;
  LedgerState ledger;
  QuotaState quota;
  RelationState relation;
  KeyAuditState key_audit;
  WatchReviewState watch_review;
  CheckpointState checkpoints;
  PolicyState policy;
  std::uint64_t epoch{};
  std::uint64_t last_digest{};
  std::vector<std::string> events;
};

Result parse_script(const std::uint8_t* data, std::size_t size,
                    std::vector<Command>& commands);
Result execute_script(const std::uint8_t* data, std::size_t size,
                      Profile profile, std::uint64_t* digest = nullptr);
Result execute_commands(Store& store, const std::vector<Command>& commands,
                        Profile profile);

Result add_document(Store& store, std::uint32_t id, std::string_view title,
                    std::vector<std::uint8_t> body);
Result create_title_view(Store& store, std::uint32_t id);
Result rename_document(Store& store, std::uint32_t id, std::string_view title);
Result emit_manifest(Store& store);

Result add_page(Store& store, std::uint32_t id,
                std::vector<std::uint8_t> bytes);
Result create_cursor(Store& store, std::uint32_t id, std::size_t offset);
Result compact_pages(Store& store);
Result scan_cursor(Store& store, std::size_t cursor_index);

Result add_key(Store& store, std::uint32_t id,
               std::vector<std::uint8_t> material);
Result rotate_key(Store& store, std::uint32_t old_id, std::uint32_t new_id,
                  std::vector<std::uint8_t> material);
Result lease_key(Store& store, std::uint32_t lease_id, std::uint32_t key_id,
                 std::string_view scope);
Result release_key(Store& store, std::uint32_t lease_id);
Result audit_keyring(Store& store, std::string_view view);

Result add_node(Store& store, std::uint32_t id, std::uint64_t value);
Result watch_node(Store& store, std::uint32_t id);
Result drop_node(Store& store, std::uint32_t id);
Result audit_watches(Store& store);
Result snapshot_watches(Store& store, std::uint32_t tag);
Result review_watch_state(Store& store, std::string_view mode);

Result define_schema(Store& store, std::uint32_t row_size,
                     std::vector<Field> fields);
Result install_row(Store& store, std::vector<std::uint8_t> row);
Result project_row(Store& store);

Result define_links(Store& store, std::vector<std::uint32_t> targets);
Result select_link(Store& store, std::uint32_t index);
Result export_selected_link(Store& store);

Result configure_signatures(Store& store, std::uint32_t allocated,
                            std::uint32_t requested);
Result finalize_session(Store& store);
Result write_signatures(Store& store);

Result configure_vector(Store& store, std::uint32_t count,
                        std::uint32_t width,
                        std::vector<std::uint8_t> source);
Result decode_vector(Store& store);

Result configure_label(Store& store, std::uint32_t declared,
                       std::string text);
Result render_label(Store& store);

Result choose_palette(Store& store, std::uint32_t index);
Result emit_palette_color(Store& store);

std::uint64_t hash_bytes(const std::uint8_t* data, std::size_t size,
                         std::uint64_t seed = 1469598103934665603ULL);
std::vector<std::uint8_t> decode_hex(std::string_view text, bool* ok = nullptr);
std::string encode_hex(const std::uint8_t* data, std::size_t size);
bool parse_u32(std::string_view text, std::uint32_t& value);
bool parse_u64(std::string_view text, std::uint64_t& value);

}  // namespace vesper
