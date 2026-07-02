#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 atlas 010203\n"
      "PAGE 10 aabbccdd\n"
      "SEGMENT 7 2\n"
      "ENTRY DOC 0102\n"
      "ENTRY PAGE aabb\n"
      "SEALSEG\n"
      "REPLAY 7\n",
      "seal and replay a simple segment");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 3 alpha 0102\n"
      "DOC 4 beta 0304\n"
      "PAGE 8 aabbccdd\n"
      "SNAPSHOT 9\n"
      "RESTORE 9\n"
      "SNAPINFO\n",
      "capture and restore snapshot catalog");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 4 aabbccddeeff\n"
      "CACHE 4 4\n"
      "TOUCH 0\n"
      "EVICT 32\n",
      "cache page window and touch it");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "ENTRY DOC aa\n",
      vesper::Status::invalid_state,
      "cannot append segment entry without open segment");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "SEGMENT 0 1\n",
      vesper::Status::limit,
      "segment id zero is rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "SEGMENT 1 1\n"
      "ENTRY bad-kind aa\n",
      vesper::Status::limit,
      "segment entry kind must be normalized");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "SEGMENT 2 2\n"
      "ENTRY DOC aa\n"
      "SEALSEG\n",
      vesper::Status::invalid_state,
      "segment refuses mismatched entry count");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "REPLAY 99\n",
      vesper::Status::not_found,
      "replay rejects unknown segment");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "RESTORE 1\n",
      vesper::Status::not_found,
      "restore rejects missing snapshot");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "SNAPINFO\n",
      vesper::Status::not_found,
      "snapshot inspection needs captured state");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "PAGE 1 aa\n"
      "CACHE 1 2\n",
      vesper::Status::limit,
      "cache width cannot exceed page size");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "TOUCH 0\n",
      vesper::Status::limit,
      "cannot touch absent cache window");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "EVICT 0\n",
      vesper::Status::ok,
      "evict to zero bytes is a no-op without windows");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 11 north 00010203\n"
      "PAGE 21 aabbccddeeff\n"
      "SEGMENT 3 3\n"
      "ENTRY DOC 0102\n"
      "ENTRY PAGE aabbcc\n"
      "ENTRY META 99\n"
      "SEALSEG\n"
      "REPLAY 3\n"
      "SNAPSHOT 6\n"
      "RESTORE 6\n"
      "SNAPINFO\n",
      "storage workflow yields digest");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "PAGE 7 00010203040506\n"
      "PAGE 8 0708090a0b0c0d\n"
      "CACHE 7 3\n"
      "CACHE 8 4\n"
      "TOUCH 1\n"
      "EVICT 16\n",
      "cache sequence yields digest");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 40 gamma 01020304\n"
      "DOC 41 delta 05060708\n"
      "PAGE 15 aabbccdd\n"
      "PAGE 16 eeff0011\n"
      "SNAPSHOT 4\n"
      "SEGMENT 8 2\n"
      "ENTRY DOC 1234\n"
      "ENTRY PAGE aabb\n"
      "SEALSEG\n"
      "REPLAY 8\n"
      "RESTORE 4\n",
      "snapshot restore coexists with segment replay");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 9 aabbccddeeff0011\n"
      "PAGE 10 2233445566778899\n"
      "CACHE 9 4\n"
      "CACHE 10 5\n"
      "TOUCH 0\n"
      "TOUCH 1\n"
      "EVICT 10\n",
      "multiple cache windows are maintained");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 root 0101\n"
      "DOC 2 leaf 0202\n"
      "PAGE 1 aa55aa55\n"
      "PAGE 2 55aa55aa\n"
      "SNAPSHOT 1\n"
      "SNAPSHOT 2\n"
      "RESTORE 1\n"
      "SNAPINFO\n"
      "RESTORE 2\n",
      "multiple snapshots can be restored");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "PAGE 3 aabbcc\n"
      "CACHE 3 2\n"
      "TOUCH 9\n",
      vesper::Status::limit,
      "touch validates slot bound after cache creation");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "SEGMENT 13 1\n"
      "ENTRY META abcd\n"
      "SEALSEG\n"
      "REPLAY 13\n",
      "single entry segment works");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 19 south 01020304\n"
      "SNAPSHOT 3\n"
      "DOC 20 east 05060708\n"
      "RESTORE 3\n"
      "SNAPINFO\n",
      "restore rolls back later additions");

  return vesper_test::finish(ctx);
}
