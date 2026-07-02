#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 801 archive-node 0a0b0c0d\n"
      "PAGE 801 1020304050607080\n"
      "NODE 80 6400\n"
      "WATCH 80\n"
      "AUDIT\n"
      "DIGEST nodes 0\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE archive 5\n"
      "CHECKTICKET 0\n"
      "SAMPLE nodes 1\n"
      "FOLD\n"
      "SNAPSHOT 80\n"
      "LEDGER query\n"
      "FOLDLEDGER\n"
      "QUOTA 9\n"
      "ENFORCE nodes\n"
      "RELATE nodes\n"
      "WALKREL 80 1\n"
      "BLOOM 6 2\n"
      "PROBE archive-node\n"
      "PLAN graph 6\n"
      "EVAL\n"
      "METRICS graph\n"
      "REDUCE\n"
      "NORMALIZE 801\n"
      "TOKENS 4\n"
      "ROLE archive\n"
      "GRANT archive inspect\n"
      "CHECK archive inspect\n"
      "TIME 80 8000\n"
      "WINDOW 7900 8100\n"
      "RETAIN 1\n"
      "INSPECT security\n",
      "story suite g workflow 1");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "QUOTA 4\n"
      "ENFORCE mystery\n",
      vesper::Status::not_found,
      "story suite g missing quota bucket");

  return vesper_test::finish(ctx);
}
