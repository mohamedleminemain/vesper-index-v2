cmake_minimum_required(VERSION 3.16)
project(vesper_index VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(VESPER_BUILD_EXPLORATORY_TESTS
  "Build the larger extended regression test inventory" OFF)

set(VESPER_SOURCES
  src/audit.cpp
  src/acl.cpp
  src/blocks.cpp
  src/bytes.cpp
  src/codec.cpp
  src/checksum.cpp
  src/compaction.cpp
  src/cache.cpp
  src/checkpoints.cpp
  src/delta.cpp
  src/diff.cpp
  src/engine.cpp
  src/export.cpp
  src/bloom.cpp
  src/inspect.cpp
  src/key_audit.cpp
  src/ledger.cpp
  src/label_format.cpp
  src/manifest.cpp
  src/metrics.cpp
  src/normalizer.cpp
  src/palette.cpp
  src/palette_header.cpp
  src/parser.cpp
  src/planner.cpp
  src/projection.cpp
  src/policy.cpp
  src/quotas.cpp
  src/query.cpp
  src/references.cpp
  src/rekey.cpp
  src/relations.cpp
  src/renderer.cpp
  src/merge.cpp
  src/sampler.cpp
  src/schema.cpp
  src/session.cpp
  src/shards.cpp
  src/signature.cpp
  src/skip_index.cpp
  src/snapshot.cpp
  src/store.cpp
  src/tickets.cpp
  src/timeline.cpp
  src/transaction.cpp
  src/vector_format.cpp
  src/wal.cpp
  src/watch_review.cpp
)

add_library(vesper_index ${VESPER_SOURCES})
target_include_directories(vesper_index PUBLIC include)
target_compile_options(vesper_index PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>)

add_executable(vesper-replay tools/replay.cpp)
target_link_libraries(vesper-replay PRIVATE vesper_index)
add_executable(vesper-inspect tools/inspect_store.cpp)
target_link_libraries(vesper-inspect PRIVATE vesper_index)
add_executable(vesper-journal-stats tools/journal_stats.cpp)
target_link_libraries(vesper-journal-stats PRIVATE vesper_index)
add_executable(vesper-snapshot-dump tools/snapshot_dump.cpp)
target_link_libraries(vesper-snapshot-dump PRIVATE vesper_index)
add_executable(vesper-segment-dump tools/segment_dump.cpp)
target_link_libraries(vesper-segment-dump PRIVATE vesper_index)
add_executable(vesper-script-profile tools/script_profile.cpp)
target_link_libraries(vesper-script-profile PRIVATE vesper_index)
add_executable(vesper-seed-merge tools/seed_merge.cpp)
target_link_libraries(vesper-seed-merge PRIVATE vesper_index)
add_executable(vesper-state-digest-report tools/state_digest_report.cpp)
target_link_libraries(vesper-state-digest-report PRIVATE vesper_index)
add_executable(vesper-story-report tools/story_report.cpp)
target_link_libraries(vesper-story-report PRIVATE vesper_index)
add_executable(vesper-ledger-report tools/ledger_report.cpp)
target_link_libraries(vesper-ledger-report PRIVATE vesper_index)
add_executable(vesper-key-watch-report tools/key_watch_report.cpp)
target_link_libraries(vesper-key-watch-report PRIVATE vesper_index)
add_executable(vesper-cache-dump tools/cache_dump.cpp)
target_link_libraries(vesper-cache-dump PRIVATE vesper_index)
add_executable(vesper-checkpoint-dump tools/checkpoint_dump.cpp)
target_link_libraries(vesper-checkpoint-dump PRIVATE vesper_index)
add_executable(vesper-checkpoint-report tools/checkpoint_report.cpp)
target_link_libraries(vesper-checkpoint-report PRIVATE vesper_index)
add_executable(vesper-acl-dump tools/acl_dump.cpp)
target_link_libraries(vesper-acl-dump PRIVATE vesper_index)
add_executable(vesper-block-dump tools/block_dump.cpp)
target_link_libraries(vesper-block-dump PRIVATE vesper_index)
add_executable(vesper-bloom-dump tools/bloom_dump.cpp)
target_link_libraries(vesper-bloom-dump PRIVATE vesper_index)
add_executable(vesper-diff-dump tools/diff_dump.cpp)
target_link_libraries(vesper-diff-dump PRIVATE vesper_index)
add_executable(vesper-digest-dump tools/digest_dump.cpp)
target_link_libraries(vesper-digest-dump PRIVATE vesper_index)
add_executable(vesper-ledger-dump tools/ledger_dump.cpp)
target_link_libraries(vesper-ledger-dump PRIVATE vesper_index)
add_executable(vesper-metrics-dump tools/metrics_dump.cpp)
target_link_libraries(vesper-metrics-dump PRIVATE vesper_index)
add_executable(vesper-merge-dump tools/merge_dump.cpp)
target_link_libraries(vesper-merge-dump PRIVATE vesper_index)
add_executable(vesper-normalize-dump tools/normalize_dump.cpp)
target_link_libraries(vesper-normalize-dump PRIVATE vesper_index)
add_executable(vesper-plan-dump tools/plan_dump.cpp)
target_link_libraries(vesper-plan-dump PRIVATE vesper_index)
add_executable(vesper-policy-dump tools/policy_dump.cpp)
target_link_libraries(vesper-policy-dump PRIVATE vesper_index)
add_executable(vesper-key-audit-dump tools/key_audit_dump.cpp)
target_link_libraries(vesper-key-audit-dump PRIVATE vesper_index)
add_executable(vesper-quota-dump tools/quota_dump.cpp)
target_link_libraries(vesper-quota-dump PRIVATE vesper_index)
add_executable(vesper-quota-report tools/quota_report.cpp)
target_link_libraries(vesper-quota-report PRIVATE vesper_index)
add_executable(vesper-relation-dump tools/relation_dump.cpp)
target_link_libraries(vesper-relation-dump PRIVATE vesper_index)
add_executable(vesper-relation-report tools/relation_report.cpp)
target_link_libraries(vesper-relation-report PRIVATE vesper_index)
add_executable(vesper-sample-dump tools/sample_dump.cpp)
target_link_libraries(vesper-sample-dump PRIVATE vesper_index)
add_executable(vesper-shard-dump tools/shard_dump.cpp)
target_link_libraries(vesper-shard-dump PRIVATE vesper_index)
add_executable(vesper-skip-dump tools/skip_dump.cpp)
target_link_libraries(vesper-skip-dump PRIVATE vesper_index)
add_executable(vesper-ticket-dump tools/ticket_dump.cpp)
target_link_libraries(vesper-ticket-dump PRIVATE vesper_index)
add_executable(vesper-timeline-dump tools/timeline_dump.cpp)
target_link_libraries(vesper-timeline-dump PRIVATE vesper_index)
add_executable(vesper-watch-review-dump tools/watch_review_dump.cpp)
target_link_libraries(vesper-watch-review-dump PRIVATE vesper_index)

include(CTest)
if(BUILD_TESTING)
  function(vesper_add_test target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PRIVATE vesper_index)
    add_test(NAME ${target} COMMAND ${target})
  endfunction()

  vesper_add_test(vesper-engine-tests tests/engine_tests.cpp)

  if(VESPER_BUILD_EXPLORATORY_TESTS)
    vesper_add_test(vesper-storage-workflow-tests tests/storage_workflow_tests.cpp)
    vesper_add_test(vesper-query-engine-tests tests/query_engine_tests.cpp)
    vesper_add_test(vesper-codec-pipeline-tests tests/codec_pipeline_tests.cpp)
    vesper_add_test(vesper-access-policy-tests tests/access_policy_tests.cpp)
    vesper_add_test(vesper-end-to-end-regression-tests tests/end_to_end_regression_tests.cpp)
    vesper_add_test(vesper-quota-limit-tests tests/quota_limit_tests.cpp)
    vesper_add_test(vesper-archive-replay-tests tests/archive_replay_tests.cpp)
    vesper_add_test(vesper-audit-query-tests tests/audit_query_tests.cpp)
    vesper_add_test(vesper-render-codec-tests tests/render_codec_tests.cpp)
    vesper_add_test(vesper-signing-session-tests tests/signing_session_tests.cpp)
    vesper_add_test(vesper-integration-journal-tests tests/integration_journal_tests.cpp)
    vesper_add_test(vesper-tooling-roundtrip-tests tests/tooling_roundtrip_tests.cpp)
    vesper_add_test(vesper-manifest-replay-tests tests/manifest_replay_tests.cpp)
    vesper_add_test(vesper-snapshot-replay-tests tests/snapshot_replay_tests.cpp)
    vesper_add_test(vesper-planner-projection-tests tests/planner_projection_tests.cpp)
    vesper_add_test(vesper-checkpoint-storage-tests tests/checkpoint_storage_tests.cpp)
    vesper_add_test(vesper-security-audit-tests tests/security_audit_tests.cpp)
    vesper_add_test(vesper-digest-merge-ticket-tests tests/digest_merge_ticket_tests.cpp)
    vesper_add_test(vesper-sample-diff-tests tests/sample_diff_tests.cpp)
    vesper_add_test(vesper-extended-workflow-tests tests/extended_workflow_tests.cpp)
    vesper_add_test(vesper-ledger-quota-relation-tests tests/ledger_quota_relation_tests.cpp)
    vesper_add_test(vesper-checkpoint-policy-tests tests/checkpoint_policy_tests.cpp)
    vesper_add_test(vesper-checkpoint-policy-regression-tests tests/checkpoint_policy_regression_tests.cpp)
    vesper_add_test(vesper-policy-smoke-tests tests/policy_smoke_tests.cpp)
    vesper_add_test(vesper-key-audit-tests tests/key_audit_tests.cpp)
    vesper_add_test(vesper-key-watch-activity-tests tests/key_watch_activity_tests.cpp)
    vesper_add_test(vesper-watch-review-tests tests/watch_review_tests.cpp)
    vesper_add_test(vesper-compaction-regression-tests tests/compaction_regression_tests.cpp)
    vesper_add_test(vesper-schema-regression-tests tests/schema_regression_tests.cpp)
    vesper_add_test(vesper-watch-regression-tests tests/watch_regression_tests.cpp)
    vesper_add_test(vesper-manifest-regression-tests tests/manifest_regression_tests.cpp)
    vesper_add_test(vesper-ledger-regression-tests tests/ledger_regression_tests.cpp)
    vesper_add_test(vesper-replay-regression-tests tests/replay_regression_tests.cpp)
    vesper_add_test(vesper-archive-ingest-tests tests/archive_ingest_tests.cpp)
    vesper_add_test(vesper-archive-checkpoint-tests tests/archive_checkpoint_tests.cpp)
    vesper_add_test(vesper-archive-relation-tests tests/archive_relation_tests.cpp)
    vesper_add_test(vesper-archive-projection-tests tests/archive_projection_tests.cpp)
    vesper_add_test(vesper-archive-policy-tests tests/archive_policy_tests.cpp)
    vesper_add_test(vesper-archive-snapshot-tests tests/archive_snapshot_tests.cpp)
    vesper_add_test(vesper-archive-compaction-tests tests/archive_compaction_tests.cpp)
    vesper_add_test(vesper-archive-metrics-tests tests/archive_metrics_tests.cpp)
    vesper_add_test(vesper-archive-cache-tests tests/archive_cache_tests.cpp)
    vesper_add_test(vesper-archive-export-tests tests/archive_export_tests.cpp)
    vesper_add_test(vesper-archive-ticket-tests tests/archive_ticket_tests.cpp)
    vesper_add_test(vesper-archive-digest-tests tests/archive_digest_tests.cpp)
    vesper_add_test(vesper-archive-timeline-tests tests/archive_timeline_tests.cpp)
    vesper_add_test(vesper-archive-summary-tests tests/archive_summary_tests.cpp)
    vesper_add_test(vesper-release-regression-tests tests/release_regression_tests.cpp)
  endif()
endif()
