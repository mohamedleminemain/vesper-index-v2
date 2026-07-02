#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 501 alpha-road 01020304\n"
      "PAGE 501 0011223344556677\n"
      "SEGMENT 50 2\n"
      "ENTRY DOC aa\n"
      "ENTRY PAGE bb\n"
      "SEALSEG\n"
      "REPLAY 50\n"
      "SNAPSHOT 50\n"
      "DIGEST doc 501\n"
      "DIGEST page 501\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE alpha 6\n"
      "RENEWTICKET 0 4\n"
      "CHECKTICKET 0\n"
      "SAMPLE all 6\n"
      "FOLD\n"
      "LEDGER all\n"
      "FOLDLEDGER\n"
      "QUOTA 8\n"
      "ENFORCE documents\n"
      "LINKS 11 22 33\n"
      "RELATE mixed\n"
      "WALKREL 0 3\n"
      "BLOOM 6 2\n"
      "PROBE alpha-road\n"
      "INDEX 2 2\n"
      "SEEK 328693\n"
      "PLAN mixed 10\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 501\n"
      "TOKENS 4\n"
      "PACK 8\n"
      "UNPACK\n"
      "PALETTE 6\n"
      "COLOR\n"
      "LABEL 5 north\n"
      "RENDER\n"
      "ROLE alpha\n"
      "GRANT alpha inspect\n"
      "CHECK alpha inspect\n"
      "TIME 50 5000\n"
      "WINDOW 4900 5100\n"
      "RETAIN 1\n"
      "SHARD 4\n"
      "REBALANCE 14\n"
      "SHARDINFO\n"
      "INSPECT storage\n",
      "story suite d workflow 1");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 502 beta-road 11121314\n"
      "DOC 503 gamma-road 21222324\n"
      "SNAPSHOT 51\n"
      "PAGE 502 8899aabbccdd\n"
      "SNAPSHOT 52\n"
      "DIFFSNAP 51 52\n"
      "DIGEST events 0\n"
      "VERIFY 7\n"
      "MERGE docs\n"
      "NEXT 2\n"
      "ISSUE beta 5\n"
      "CHECKTICKET 0\n"
      "SAMPLE docs 2\n"
      "FOLD\n"
      "LEDGER archive\n"
      "FOLDLEDGER\n"
      "QUOTA 5\n"
      "ENFORCE events\n"
      "RELATE docs\n"
      "WALKREL 502 1\n"
      "INSPECT catalog\n",
      "story suite d workflow 2");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "RELATE links\n"
      "WALKREL 7 2\n",
      vesper::Status::invalid_state,
      "story suite d invalid relation walk");

  return vesper_test::finish(ctx);
}
