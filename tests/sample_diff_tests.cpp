#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 alpha 01020304\n"
      "PAGE 1 aabbccdd\n"
      "NODE 1 99\n"
      "SAMPLE all 8\n"
      "FOLD\n",
      "sample all and fold");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 2 beta 02030405\n"
      "SNAPSHOT 1\n"
      "DOC 3 gamma 03040506\n"
      "SNAPSHOT 2\n"
      "DIFFSNAP 1 2\n",
      "diff two snapshots");

  vesper_test::expect_status(
      ctx,
      "VSP1\nFOLD\n",
      vesper::Status::invalid_state,
      "fold requires sampled rows");

  vesper_test::expect_status(
      ctx,
      "VSP1\nSAMPLE odd 4\n",
      vesper::Status::unsupported,
      "sample rejects unknown scope");

  vesper_test::expect_status(
      ctx,
      "VSP1\nDIFFSNAP 1 2\n",
      vesper::Status::not_found,
      "diff requires captured snapshots");

  vesper_test::expect_status(
      ctx,
      "VSP1\nSAMPLE docs 0\n",
      vesper::Status::limit,
      "sample limit must be positive");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 4 delta 10111213\n"
      "PAGE 2 44556677\n"
      "NODE 2 1000\n"
      "SAMPLE all 5\n"
      "FOLD\n"
      "SNAPSHOT 3\n"
      "DOC 5 epsilon 14151617\n"
      "SNAPSHOT 4\n"
      "DIFFSNAP 3 4\n",
      "sample and diff digest path");

  return vesper_test::finish(ctx);
}
