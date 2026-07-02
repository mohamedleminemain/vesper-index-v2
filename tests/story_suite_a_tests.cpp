#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 201 alpha 01020304\n"
      "VIEW 201\n"
      "PAGE 201 aabbccddeeff\n"
      "CURSOR 201 2\n"
      "SCAN 0\n"
      "DIGEST doc 201\n"
      "DIGEST page 201\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE analyst 4\n"
      "CHECKTICKET 0\n"
      "SAMPLE all 8\n"
      "FOLD\n"
      "SNAPSHOT 20\n"
      "LEDGER all\n"
      "FOLDLEDGER\n"
      "QUOTA 8\n"
      "ENFORCE documents\n"
      "LINKS 3 6 9\n"
      "RELATE mixed\n"
      "WALKREL 0 3\n"
      "BLOOM 8 3\n"
      "PROBE alpha\n"
      "PLAN mixed 12\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 201\n"
      "TOKENS 3\n"
      "PACK 6\n"
      "UNPACK\n"
      "ROLE guard\n"
      "GRANT guard inspect\n"
      "CHECK guard inspect\n"
      "TIME 20 2000\n"
      "WINDOW 1900 2100\n"
      "RETAIN 1\n"
      "SHARD 4\n"
      "SHARDINFO\n"
      "INSPECT catalog\n",
      "story suite a workflow 1");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 202 beta 11121314\n"
      "PAGE 202 001122334455\n"
      "SEGMENT 20 2\n"
      "ENTRY DOC aa\n"
      "ENTRY PAGE bb\n"
      "SEALSEG\n"
      "REPLAY 20\n"
      "SNAPSHOT 21\n"
      "DOC 203 gamma 21222324\n"
      "SNAPSHOT 22\n"
      "DIFFSNAP 21 22\n"
      "LEDGER archive\n"
      "FOLDLEDGER\n"
      "QUOTA 6\n"
      "ENFORCE events\n"
      "ISSUE writer 6\n"
      "RENEWTICKET 0 5\n"
      "CHECKTICKET 0\n"
      "SAMPLE docs 2\n"
      "FOLD\n"
      "RELATE docs\n"
      "WALKREL 202 1\n"
      "MERGE docs\n"
      "NEXT 2\n"
      "INSPECT storage\n",
      "story suite a workflow 2");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 204 delta 31323334\n"
      "SCHEMA 4 a 0 2 b 2 2\n"
      "ROW 01020304\n"
      "PROJECT\n"
      "DIGEST events 0\n"
      "VERIFY 9\n"
      "NODE 20 400\n"
      "WATCH 20\n"
      "AUDIT\n"
      "SAMPLE nodes 1\n"
      "FOLD\n"
      "RELATE nodes\n"
      "WALKREL 20 1\n"
      "PLAN graph 7\n"
      "EVAL\n"
      "METRICS graph\n"
      "REDUCE\n"
      "INSPECT query\n",
      "story suite a workflow 3");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "LEDGER all\n"
      "WALKREL 1 1\n",
      vesper::Status::invalid_state,
      "story suite a relation requires build");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "DOC 205 epsilon 41424344\n"
      "QUOTA 0\n",
      vesper::Status::limit,
      "story suite a quota limit");

  return vesper_test::finish(ctx);
}
