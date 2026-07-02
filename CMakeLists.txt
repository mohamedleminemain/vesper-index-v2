cmake_minimum_required(VERSION 3.16)
project(vesper_index VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(VESPER_BUILD_EXPLORATORY_TESTS
  "Build the larger exploratory state-coverage test inventory" OFF)

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
    vesper_add_test(vesper-storage-tests tests/storage_subsystem_tests.cpp)
    vesper_add_test(vesper-query-tests tests/query_subsystem_tests.cpp)
    vesper_add_test(vesper-codec-tests tests/codec_subsystem_tests.cpp)
    vesper_add_test(vesper-security-tests tests/security_subsystem_tests.cpp)
    vesper_add_test(vesper-mixed-matrix-tests tests/mixed_matrix_tests.cpp)
    vesper_add_test(vesper-limits-matrix-tests tests/limits_matrix_tests.cpp)
    vesper_add_test(vesper-archive-matrix-tests tests/archive_matrix_tests.cpp)
    vesper_add_test(vesper-query-matrix-tests tests/query_matrix_tests.cpp)
    vesper_add_test(vesper-codec-matrix-tests tests/codec_matrix_tests.cpp)
    vesper_add_test(vesper-session-matrix-tests tests/session_matrix_tests.cpp)
    vesper_add_test(vesper-integration-journal-tests tests/integration_journal_tests.cpp)
    vesper_add_test(vesper-tooling-roundtrip-tests tests/tooling_roundtrip_tests.cpp)
    vesper_add_test(vesper-replay-catalog-a-tests tests/replay_catalog_a_tests.cpp)
    vesper_add_test(vesper-replay-catalog-b-tests tests/replay_catalog_b_tests.cpp)
    vesper_add_test(vesper-planner-catalog-tests tests/planner_catalog_tests.cpp)
    vesper_add_test(vesper-storage-catalog-tests tests/storage_catalog_tests.cpp)
    vesper_add_test(vesper-security-catalog-tests tests/security_catalog_tests.cpp)
    vesper_add_test(vesper-digest-merge-ticket-tests tests/digest_merge_ticket_tests.cpp)
    vesper_add_test(vesper-sample-diff-tests tests/sample_diff_tests.cpp)
    vesper_add_test(vesper-advanced-matrix-tests tests/advanced_matrix_tests.cpp)
    vesper_add_test(vesper-ledger-quota-relation-tests tests/ledger_quota_relation_tests.cpp)
    vesper_add_test(vesper-checkpoint-policy-tests tests/checkpoint_policy_tests.cpp)
    vesper_add_test(vesper-checkpoint-policy-matrix-tests tests/checkpoint_policy_matrix_tests.cpp)
    vesper_add_test(vesper-policy-smoke-tests tests/policy_smoke_tests.cpp)
    vesper_add_test(vesper-key-audit-tests tests/key_audit_tests.cpp)
    vesper_add_test(vesper-key-watch-story-tests tests/key_watch_story_tests.cpp)
    vesper_add_test(vesper-watch-review-tests tests/watch_review_tests.cpp)
    vesper_add_test(vesper-deep-suite-a-tests tests/deep_suite_a_tests.cpp)
    vesper_add_test(vesper-deep-suite-b-tests tests/deep_suite_b_tests.cpp)
    vesper_add_test(vesper-deep-suite-c-tests tests/deep_suite_c_tests.cpp)
    vesper_add_test(vesper-deep-suite-d-tests tests/deep_suite_d_tests.cpp)
    vesper_add_test(vesper-deep-suite-e-tests tests/deep_suite_e_tests.cpp)
    vesper_add_test(vesper-deep-suite-f-tests tests/deep_suite_f_tests.cpp)
    vesper_add_test(vesper-story-suite-a-tests tests/story_suite_a_tests.cpp)
    vesper_add_test(vesper-story-suite-b-tests tests/story_suite_b_tests.cpp)
    vesper_add_test(vesper-story-suite-c-tests tests/story_suite_c_tests.cpp)
    vesper_add_test(vesper-story-suite-d-tests tests/story_suite_d_tests.cpp)
    vesper_add_test(vesper-story-suite-e-tests tests/story_suite_e_tests.cpp)
    vesper_add_test(vesper-story-suite-f-tests tests/story_suite_f_tests.cpp)
    vesper_add_test(vesper-story-suite-g-tests tests/story_suite_g_tests.cpp)
    vesper_add_test(vesper-story-suite-h-tests tests/story_suite_h_tests.cpp)
    vesper_add_test(vesper-story-suite-i-tests tests/story_suite_i_tests.cpp)
    vesper_add_test(vesper-story-suite-j-tests tests/story_suite_j_tests.cpp)
    vesper_add_test(vesper-story-suite-k-tests tests/story_suite_k_tests.cpp)
    vesper_add_test(vesper-story-suite-l-tests tests/story_suite_l_tests.cpp)
    vesper_add_test(vesper-story-suite-m-tests tests/story_suite_m_tests.cpp)
    vesper_add_test(vesper-story-suite-n-tests tests/story_suite_n_tests.cpp)
    vesper_add_test(vesper-final-story-suite-tests tests/final_story_suite_tests.cpp)
  endif()
endif()
