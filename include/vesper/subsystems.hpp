#pragma once

#include <cstddef>
#include <cstdint>
#include <deque>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace vesper {

struct SegmentEntry {
  std::uint32_t ordinal{};
  std::string kind;
  std::vector<std::uint8_t> payload;
  std::uint64_t checksum{};
  std::uint64_t digest{};
};

struct SegmentImage {
  std::uint32_t segment_id{};
  std::uint32_t expected_entries{};
  std::vector<SegmentEntry> entries;
  std::uint64_t seal_digest{};
};

struct SegmentState {
  std::uint32_t active_segment{};
  std::uint32_t expected_entries{};
  std::uint64_t sequence{};
  bool open{};
  std::vector<SegmentEntry> pending;
  std::vector<SegmentImage> sealed;
  std::unordered_map<std::uint32_t, std::size_t> positions;
  std::vector<std::uint64_t> replay_history;
};

struct SnapshotDocument {
  std::uint32_t id{};
  std::string title;
  std::vector<std::uint8_t> body;
};

struct SnapshotPage {
  std::uint32_t id{};
  std::vector<std::uint8_t> bytes;
  std::uint64_t generation{};
};

struct SnapshotView {
  std::uint32_t snapshot_id{};
  std::vector<SnapshotDocument> documents;
  std::vector<SnapshotPage> pages;
  std::vector<std::pair<std::uint32_t, std::uint64_t>> nodes;
  std::vector<std::pair<std::uint32_t, std::string>> titles;
  std::uint64_t digest{};
};

struct SnapshotState {
  std::vector<SnapshotView> captured;
  std::optional<std::uint32_t> current;
  std::uint64_t restore_count{};
};

struct BloomProbe {
  std::string token;
  std::vector<std::uint32_t> slots;
  bool maybe_present{};
};

struct BloomFilterState {
  std::vector<std::uint64_t> words;
  std::vector<BloomProbe> probes;
  std::uint32_t hash_functions{};
  std::uint32_t inserted{};
  std::uint64_t seed{};
  bool ready{};
};

struct SkipIndexPoint {
  std::uint32_t key{};
  std::uint32_t page_id{};
  std::uint32_t offset{};
  std::uint32_t span{};
};

struct SkipIndexLayer {
  std::vector<SkipIndexPoint> points;
};

struct SkipIndexState {
  std::vector<SkipIndexLayer> layers;
  std::uint32_t stride{};
  std::uint32_t search_key{};
  std::optional<SkipIndexPoint> last_match;
  bool built{};
};

struct CacheWindow {
  std::uint32_t page_id{};
  std::size_t offset{};
  std::vector<std::uint8_t> bytes;
  std::uint64_t stamp{};
  std::uint32_t hits{};
  bool pinned{};
};

struct CacheState {
  std::vector<CacheWindow> windows;
  std::deque<std::size_t> lru;
  std::size_t target_bytes{256};
  std::size_t total_bytes{};
  std::uint64_t stamp{};
  std::uint64_t evictions{};
  std::optional<std::size_t> last_window;
};

struct PlanStep {
  std::string name;
  std::uint64_t estimated_cost{};
  std::uint32_t fanout{};
  std::uint32_t cardinality{};
  bool blocking{};
};

struct QueryPlanState {
  std::string mode;
  std::vector<PlanStep> steps;
  std::uint64_t total_cost{};
  std::uint32_t limit{};
  std::uint64_t result_digest{};
  bool ready{};
};

struct MetricBucket {
  std::uint64_t lower{};
  std::uint64_t upper{};
  std::uint64_t count{};
  std::uint64_t weight{};
};

struct MetricsState {
  std::string scope;
  std::vector<MetricBucket> buckets;
  std::vector<std::uint64_t> samples;
  std::uint64_t reduced{};
  std::uint64_t minimum{};
  std::uint64_t maximum{};
  std::uint64_t average{};
  bool collected{};
};

struct NormalizedToken {
  std::string raw;
  std::string normalized;
  std::uint32_t count{};
  std::uint32_t source_id{};
};

struct NormalizerState {
  std::vector<NormalizedToken> tokens;
  std::vector<std::string> emitted;
  std::uint64_t digest{};
  bool ready{};
};

struct RoleDefinition {
  std::string name;
  std::vector<std::string> permissions;
};

struct AclState {
  std::vector<RoleDefinition> roles;
  std::vector<std::string> checks;
  std::uint64_t digest{};
};

struct TimelineEvent {
  std::uint32_t tag{};
  std::uint64_t stamp{};
  std::uint64_t digest{};
};

struct TimelineState {
  std::vector<TimelineEvent> events;
  std::vector<TimelineEvent> window;
  std::uint64_t retained{};
  std::uint64_t digest{};
};

struct ShardEntry {
  std::uint32_t shard{};
  std::uint32_t object_id{};
  std::uint64_t weight{};
  std::string kind;
};

struct ShardState {
  std::vector<ShardEntry> placements;
  std::vector<std::uint64_t> load;
  std::uint32_t count{};
  std::uint64_t digest{};
  bool ready{};
};

struct BlockRun {
  std::uint8_t value{};
  std::uint32_t length{};
};

struct BlockChunk {
  std::uint32_t page_id{};
  std::uint32_t ordinal{};
  std::vector<BlockRun> runs;
  std::vector<std::uint8_t> flat;
  std::uint64_t digest{};
};

struct BlockState {
  std::vector<BlockChunk> chunks;
  std::uint32_t block_size{};
  std::uint64_t packed_bytes{};
  std::uint64_t unpacked_bytes{};
  bool ready{};
};

struct InspectState {
  std::vector<std::string> lines;
  std::uint64_t digest{};
};

struct DigestEntry {
  std::string kind;
  std::uint32_t object_id{};
  std::uint64_t digest{};
  std::uint64_t stamp{};
};

struct DigestState {
  std::vector<DigestEntry> entries;
  std::optional<std::uint64_t> expected;
  std::uint64_t verified{};
  bool matched{};
};

struct MergeCursorRow {
  std::uint32_t rank{};
  std::uint32_t id{};
  std::string kind;
  std::uint64_t weight{};
};

struct MergeState {
  std::vector<MergeCursorRow> rows;
  std::size_t cursor{};
  std::string mode;
  std::uint64_t digest{};
  bool prepared{};
};

struct Ticket {
  std::string subject;
  std::uint64_t issued{};
  std::uint64_t expires{};
  std::uint64_t token{};
  bool revoked{};
};

struct TicketState {
  std::vector<Ticket> tickets;
  std::uint64_t epoch{};
  std::uint64_t digest{};
};

struct SampleRow {
  std::uint32_t ordinal{};
  std::uint64_t value{};
  std::string label;
};

struct SampleState {
  std::vector<SampleRow> rows;
  std::vector<std::uint64_t> folds;
  std::string scope;
  std::uint32_t limit{};
  std::uint64_t digest{};
};

struct DiffEntry {
  std::string field;
  std::int64_t left{};
  std::int64_t right{};
};

struct DiffState {
  std::vector<DiffEntry> entries;
  std::uint32_t left_id{};
  std::uint32_t right_id{};
  std::uint64_t digest{};
  bool ready{};
};

struct LedgerEntry {
  std::uint32_t ordinal{};
  std::string tag;
  std::uint64_t value{};
};

struct LedgerState {
  std::vector<LedgerEntry> entries;
  std::uint64_t folded{};
  std::uint64_t digest{};
};

struct QuotaBucket {
  std::string name;
  std::uint64_t used{};
  std::uint64_t limit{};
  bool exceeded{};
};

struct QuotaState {
  std::vector<QuotaBucket> buckets;
  std::uint64_t digest{};
  bool prepared{};
};

struct RelationEdge {
  std::uint32_t from{};
  std::uint32_t to{};
  std::string kind;
  std::uint64_t weight{};
};

struct RelationState {
  std::vector<RelationEdge> edges;
  std::vector<std::uint32_t> walk;
  std::uint64_t digest{};
  bool prepared{};
};

struct KeyLease {
  std::uint32_t lease_id{};
  std::uint32_t key_id{};
  std::string scope;
  std::uint64_t issued_epoch{};
  std::uint64_t released_epoch{};
  std::uint64_t digest{};
  bool active{};
};

struct KeyAuditState {
  std::vector<KeyLease> leases;
  std::vector<std::string> history;
  std::optional<std::uint32_t> selected_lease;
  std::uint64_t digest{};
};

struct WatchSnapshot {
  std::uint32_t tag{};
  std::uint32_t node_id{};
  std::uint64_t value{};
  std::uint32_t relation_edges{};
  std::uint32_t timeline_hits{};
  std::uint64_t digest{};
};

struct WatchReviewState {
  std::vector<WatchSnapshot> snapshots;
  std::vector<std::string> notes;
  std::optional<std::uint32_t> anchor;
  std::uint64_t digest{};
};

struct CheckpointEntry {
  std::uint32_t id{};
  std::uint64_t epoch{};
  std::uint64_t digest{};
  std::uint32_t documents{};
  std::uint32_t pages{};
  std::uint32_t events{};
};

struct CheckpointState {
  std::vector<CheckpointEntry> entries;
  std::optional<std::uint32_t> selected;
  const char* selected_summary{};
  std::size_t selected_summary_size{};
  std::uint64_t digest{};
};

struct PolicyRule {
  std::string scope;
  std::string operation;
  std::uint64_t threshold{};
  bool enabled{};
};

struct PolicyState {
  std::vector<PolicyRule> rules;
  std::vector<std::string> evaluations;
  std::uint64_t digest{};
};

class Store;
struct Result;

Result begin_segment(Store& store, std::uint32_t segment_id,
                     std::uint32_t expected_entries);
Result append_segment(Store& store, std::string_view kind,
                      std::vector<std::uint8_t> payload);
Result seal_segment(Store& store);
Result replay_segment(Store& store, std::uint32_t segment_id);

Result capture_snapshot(Store& store, std::uint32_t snapshot_id);
Result restore_snapshot(Store& store, std::uint32_t snapshot_id);
Result inspect_snapshots(Store& store);

Result build_bloom(Store& store, std::uint32_t word_count,
                   std::uint32_t hash_count);
Result probe_bloom(Store& store, std::string_view token);

Result build_skip_index(Store& store, std::uint32_t stride,
                        std::uint32_t levels);
Result seek_skip_index(Store& store, std::uint32_t key);

Result cache_page_window(Store& store, std::uint32_t page_id,
                         std::uint32_t width);
Result touch_cache_window(Store& store, std::uint32_t slot);
Result evict_cache_windows(Store& store, std::uint32_t target_bytes);

Result build_query_plan(Store& store, std::string_view mode,
                        std::uint32_t limit);
Result evaluate_query_plan(Store& store);

Result collect_metrics(Store& store, std::string_view scope);
Result reduce_metrics(Store& store);

Result normalize_document(Store& store, std::uint32_t document_id);
Result emit_normalized_tokens(Store& store, std::uint32_t limit);

Result define_role(Store& store, std::string_view role);
Result grant_role(Store& store, std::string_view role,
                  std::string_view permission);
Result check_permission(Store& store, std::string_view role,
                        std::string_view permission);

Result record_timeline(Store& store, std::uint32_t tag, std::uint64_t stamp);
Result window_timeline(Store& store, std::uint64_t begin, std::uint64_t end);
Result retain_timeline(Store& store, std::uint32_t count);

Result assign_shards(Store& store, std::uint32_t count);
Result rebalance_shards(Store& store, std::uint32_t target_load);
Result inspect_shards(Store& store);

Result pack_blocks(Store& store, std::uint32_t block_size);
Result unpack_blocks(Store& store);

Result inspect_store(Store& store, std::string_view view);
Result capture_digest(Store& store, std::string_view kind, std::uint32_t object_id);
Result verify_digest(Store& store, std::uint64_t expected);
Result prepare_merge(Store& store, std::string_view mode);
Result next_merge(Store& store, std::uint32_t steps);
Result issue_ticket(Store& store, std::string_view subject, std::uint32_t ttl);
Result renew_ticket(Store& store, std::uint32_t index, std::uint32_t ttl);
Result check_ticket(Store& store, std::uint32_t index);
Result sample_scope(Store& store, std::string_view scope, std::uint32_t limit);
Result fold_sample(Store& store);
Result diff_snapshots(Store& store, std::uint32_t left_id, std::uint32_t right_id);
Result build_ledger(Store& store, std::string_view mode);
Result fold_ledger(Store& store);
Result prepare_quota(Store& store, std::uint32_t scale);
Result enforce_quota(Store& store, std::string_view name);
Result build_relations(Store& store, std::string_view mode);
Result walk_relations(Store& store, std::uint32_t start, std::uint32_t steps);
Result lease_key(Store& store, std::uint32_t lease_id, std::uint32_t key_id,
                 std::string_view scope);
Result release_key(Store& store, std::uint32_t lease_id);
Result audit_keyring(Store& store, std::string_view view);
Result snapshot_watches(Store& store, std::uint32_t tag);
Result review_watch_state(Store& store, std::string_view mode);
Result capture_checkpoint(Store& store, std::uint32_t id);
Result inspect_checkpoint(Store& store, std::uint32_t id);
Result define_policy(Store& store, std::string_view scope,
                     std::string_view operation, std::uint32_t threshold);
Result evaluate_policy(Store& store, std::string_view scope,
                       std::string_view operation);

}  // namespace vesper
