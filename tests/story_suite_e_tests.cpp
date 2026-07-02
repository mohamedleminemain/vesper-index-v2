#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 601 orbit-lane 01010101\n"
      "PAGE 601 aabbccddeeff0011\n"
      "CACHE 601 6\n"
      "TOUCH 0\n"
      "DIGEST page 601\n"
      "MERGE pages\n"
      "NEXT 1\n"
      "ISSUE orbit 7\n"
      "CHECKTICKET 0\n"
      "SAMPLE pages 1\n"
      "FOLD\n"
      "LEDGER codec\n"
      "FOLDLEDGER\n"
      "QUOTA 10\n"
      "ENFORCE pages\n"
      "LINKS 5 10 15 20\n"
      "RELATE links\n"
      "WALKREL 0 4\n"
      "BLOOM 7 3\n"
      "PROBE orbit-lane\n"
      "INDEX 3 2\n"
      "SEEK 394817\n"
      "PLAN pages 9\n"
      "EVAL\n"
      "METRICS pages\n"
      "REDUCE\n"
      "NORMALIZE 601\n"
      "TOKENS 2\n"
      "PACK 4\n"
      "UNPACK\n"
      "ROLE orbit\n"
      "GRANT orbit read\n"
      "CHECK orbit read\n"
      "TIME 60 6000\n"
      "WINDOW 5900 6100\n"
      "RETAIN 1\n"
      "SHARD 5\n"
      "SHARDINFO\n",
      "story suite e workflow 1");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 602 vector-ring 02020202\n"
      "SCHEMA 6 a 0 2 b 2 2 c 4 2\n"
      "ROW 010203040506\n"
      "PROJECT\n"
      "NODE 60 3600\n"
      "WATCH 60\n"
      "AUDIT\n"
      "DIGEST nodes 0\n"
      "MERGE docs\n"
      "NEXT 1\n"
      "ISSUE vector 4\n"
      "CHECKTICKET 0\n"
      "SAMPLE nodes 1\n"
      "FOLD\n"
      "LEDGER query\n"
      "FOLDLEDGER\n"
      "QUOTA 6\n"
      "ENFORCE nodes\n"
      "RELATE nodes\n"
      "WALKREL 60 1\n"
      "INSPECT query\n",
      "story suite e workflow 2");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "ISSUE orbit 0\n",
      vesper::Status::limit,
      "story suite e invalid ttl");

  return vesper_test::finish(ctx);
}
