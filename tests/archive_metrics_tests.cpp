#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 901 final-route 01010101\n"
      "PAGE 901 aa00bb00cc00dd00\n"
      "CACHE 901 4\n"
      "TOUCH 0\n"
      "DIGEST page 901\n"
      "MERGE pages\n"
      "NEXT 1\n"
      "ISSUE final 9\n"
      "CHECKTICKET 0\n"
      "SAMPLE pages 1\n"
      "FOLD\n"
      "SNAPSHOT 90\n"
      "LEDGER codec\n"
      "FOLDLEDGER\n"
      "QUOTA 10\n"
      "ENFORCE pages\n"
      "RELATE docs\n"
      "BLOOM 7 2\n"
      "PROBE final-route\n"
      "INDEX 2 2\n"
      "SEEK 590613\n"
      "PLAN pages 7\n"
      "EVAL\n"
      "METRICS pages\n"
      "REDUCE\n"
      "NORMALIZE 901\n"
      "TOKENS 2\n"
      "PACK 8\n"
      "UNPACK\n"
      "ROLE final\n"
      "GRANT final read\n"
      "CHECK final read\n"
      "TIME 90 9000\n"
      "WINDOW 8900 9100\n"
      "RETAIN 1\n"
      "SHARD 6\n"
      "SHARDINFO\n",
      "story suite h workflow 1");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "RELATE docs\n"
      "WALKREL 901 0\n",
      vesper::Status::limit,
      "story suite h invalid walk size");

  return vesper_test::finish(ctx);
}
