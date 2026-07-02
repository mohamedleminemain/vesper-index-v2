#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 401 north-west 0a0b0c0d\n"
      "PAGE 401 ffeeddccbbaa\n"
      "LINKS 10 20 30 40\n"
      "FOLLOW 2\n"
      "EXPORT\n"
      "NODE 40 1600\n"
      "WATCH 40\n"
      "AUDIT\n"
      "DIGEST links 0\n"
      "DIGEST nodes 0\n"
      "MERGE links\n"
      "NEXT 3\n"
      "ISSUE planner 7\n"
      "CHECKTICKET 0\n"
      "SAMPLE all 6\n"
      "FOLD\n"
      "SNAPSHOT 41\n"
      "LEDGER query\n"
      "FOLDLEDGER\n"
      "QUOTA 7\n"
      "ENFORCE links\n"
      "RELATE mixed\n"
      "WALKREL 0 4\n"
      "BLOOM 8 2\n"
      "PROBE link:30\n"
      "PLAN graph 11\n"
      "EVAL\n"
      "METRICS graph\n"
      "REDUCE\n"
      "NORMALIZE 401\n"
      "TOKENS 5\n"
      "ROLE graph\n"
      "GRANT graph inspect\n"
      "CHECK graph inspect\n"
      "TIME 40 4000\n"
      "WINDOW 3900 4100\n"
      "RETAIN 1\n"
      "INSPECT security\n",
      "story suite c workflow 1");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 402 ring-road 01020304\n"
      "PAGE 402 0102030405060708\n"
      "CACHE 402 5\n"
      "TOUCH 0\n"
      "PACK 8\n"
      "UNPACK\n"
      "DIGEST page 402\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE cache 4\n"
      "CHECKTICKET 0\n"
      "SAMPLE pages 1\n"
      "FOLD\n"
      "LEDGER codec\n"
      "FOLDLEDGER\n"
      "QUOTA 10\n"
      "ENFORCE pages\n"
      "RELATE docs\n"
      "PLAN pages 6\n"
      "EVAL\n"
      "INSPECT catalog\n",
      "story suite c workflow 2");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "QUOTA 5\n"
      "ENFORCE mystery\n",
      vesper::Status::not_found,
      "story suite c quota bucket check");

  return vesper_test::finish(ctx);
}
