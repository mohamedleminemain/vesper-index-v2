#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 atlas 01020304\n"
      "PAGE 1 aabbccdd\n"
      "SNAPSHOT 1\n"
      "SEGMENT 1 1\n"
      "ENTRY META aa\n"
      "SEALSEG\n"
      "BLOOM 6 2\n"
      "PROBE atlas\n"
      "PLAN docs 4\n"
      "EVAL\n"
      "INSPECT catalog\n",
      "tooling-compatible journal one");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 2 beta 11223344\n"
      "NORMALIZE 2\n"
      "TOKENS 4\n"
      "PAGE 2 aaaa5555\n"
      "PACK 4\n"
      "UNPACK\n"
      "INSPECT storage\n",
      "tooling-compatible journal two");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "ROLE analyst\n"
      "GRANT analyst read\n"
      "CHECK analyst read\n"
      "TIME 3 300\n"
      "WINDOW 200 400\n"
      "RETAIN 1\n"
      "INSPECT security\n",
      "tooling-compatible journal three");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 4 gamma 0101\n"
      "PAGE 4 0102030405\n"
      "SHARD 4\n"
      "REBALANCE 7\n"
      "SHARDINFO\n"
      "INSPECT query\n",
      "tooling-compatible journal four");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 5 delta 05060708\n"
      "PAGE 5 0a0b0c0d0e0f\n"
      "SNAPSHOT 5\n"
      "SEGMENT 5 2\n"
      "ENTRY DOC aa\n"
      "ENTRY PAGE bb\n"
      "SEALSEG\n"
      "REPLAY 5\n"
      "INSPECT storage\n",
      "tooling-compatible journal digest");

  return vesper_test::finish(ctx);
}
