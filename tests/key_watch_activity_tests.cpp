#include "test_support.hpp"

namespace {

constexpr const char* kArchiveStory =
    "VSP1\n"
    "DOC 10 north 00112233\n"
    "DOC 11 south 44556677\n"
    "KEY 5 aabbccdd\n"
    "KEYLEASE 50 5 ingest\n"
    "KEYAUDIT active\n"
    "VIEW 10\n"
    "RENAME 10 atlas\n"
    "MANIFEST\n"
    "KEYRELEASE 50\n"
    "KEYAUDIT history\n";

constexpr const char* kQueryStory =
    "VSP1\n"
    "NODE 10 100\n"
    "NODE 11 200\n"
    "WATCH 10\n"
    "RELATE nodes\n"
    "WALKREL 10 1\n"
    "TIME 10 500\n"
    "WINDOW 0 1000\n"
    "WATCHSNAP 10\n"
    "WATCHREVIEW all\n"
    "AUDIT\n";

constexpr const char* kCombinedStory =
    "VSP1\n"
    "DOC 20 west 01020304\n"
    "PAGE 20 aabbccdd\n"
    "KEY 9 11223344\n"
    "KEYLEASE 90 9 replay\n"
    "NODE 20 400\n"
    "WATCH 20\n"
    "RELATE nodes\n"
    "TIME 20 900\n"
    "WINDOW 0 1000\n"
    "WATCHSNAP 20\n"
    "CHECKPOINT 70\n"
    "CHECKVIEW 70\n"
    "POLICY checkpoints max 65535\n"
    "WATCHREVIEW timeline\n"
    "POLICYCHECK checkpoints max\n";

}  // namespace

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(ctx, kArchiveStory, "archive key story");
  vesper_test::expect_ok(ctx, kQueryStory, "query watch story");
  vesper_test::expect_ok(ctx, kCombinedStory, "combined key watch story");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 5 aabbccdd\nKEYLEASE 8 5 ingest\nKEYAUDIT mystery\n",
      vesper::Status::unsupported,
      "unknown key audit report rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nNODE 3 99\nWATCH 3\nWATCHSNAP 0\n",
      vesper::Status::limit,
      "zero watch snapshot tag rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 5 aabbccdd\nKEYLEASE 0 5 ingest\n",
      vesper::Status::limit,
      "zero lease id rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 5 aabbccdd\nKEYLEASE 4 5 ingesting_scope_that_is_far_too_long\n",
      vesper::Status::limit,
      "long lease scope rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nNODE 10 1\nWATCH 10\nWATCHSNAP 10\nWATCHREVIEW relations\n",
      vesper::Status::ok,
      "relation-free review still works");

  vesper_test::expect_status(
      ctx,
      "VSP1\nNODE 10 1\nWATCH 10\nTIME 10 5\nWINDOW 0 10\nWATCHSNAP 10\nWATCHREVIEW timeline\n",
      vesper::Status::ok,
      "timeline review works");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 3 aabbccdd\nKEYLEASE 9 3 ingest\nKEYRELEASE 9\nKEYRELEASE 9\n",
      vesper::Status::invalid_state,
      "double release rejected");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "KEY 1 aabbccdd\n"
      "KEYLEASE 1 1 ingest\n"
      "KEYAUDIT leases\n"
      "NODE 1 42\n"
      "WATCH 1\n"
      "WATCHSNAP 1\n"
      "WATCHREVIEW summary\n",
      "combined story digest");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 east 00\n"
      "KEY 2 00112233\n"
      "KEYLEASE 4 2 ingest\n"
      "PAGE 1 aabb\n"
      "CURSOR 1 0\n"
      "SCAN 0\n"
      "NODE 1 10\n"
      "WATCH 1\n"
      "WATCHSNAP 1\n"
      "WATCHREVIEW summary\n"
      "CHECKPOINT 1\n"
      "INSPECT catalog\n"
      "KEYAUDIT active\n",
      "catalog preserving story");

  return vesper_test::finish(ctx);
}
