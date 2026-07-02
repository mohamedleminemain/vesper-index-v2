#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1501 harbor 11121314\n"
      "DOC 1502 inlet 21222324\n"
      "SNAPSHOT 150\n"
      "PAGE 1501 0101010102020202\n"
      "CHECKPOINT 150\n"
      "CHECKVIEW 150\n"
      "POLICY events max 32\n"
      "POLICYCHECK events max\n"
      "SNAPSHOT 151\n"
      "DIFFSNAP 150 151\n"
      "DIGEST events 0\n"
      "MERGE docs\n"
      "NEXT 2\n"
      "ISSUE harbor 4\n"
      "CHECKTICKET 0\n"
      "SAMPLE docs 2\n"
      "FOLD\n"
      "LEDGER archive\n"
      "FOLDLEDGER\n"
      "QUOTA 6\n"
      "ENFORCE events\n"
      "RELATE docs\n"
      "WALKREL 1501 1\n"
      "PLAN docs 4\n"
      "EVAL\n"
      "METRICS docs\n"
      "REDUCE\n"
      "NORMALIZE 1501\n"
      "TOKENS 2\n"
      "ROLE harbor\n"
      "GRANT harbor inspect\n"
      "CHECK harbor inspect\n"
      "TIME 150 15000\n"
      "WINDOW 14900 15100\n"
      "RETAIN 1\n"
      "INSPECT storage\n",
      "story suite n workflow");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "POLICYCHECK pages max\n",
      vesper::Status::not_found,
      "story suite n missing policy");

  return vesper_test::finish(ctx);
}
