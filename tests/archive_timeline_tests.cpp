#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1401 ridge 01020304\n"
      "PAGE 1401 abcdef0123456789\n"
      "CHECKPOINT 140\n"
      "CHECKVIEW 140\n"
      "POLICY documents max 8\n"
      "POLICY pages max 8\n"
      "POLICYCHECK documents max\n"
      "POLICYCHECK pages max\n"
      "DIGEST doc 1401\n"
      "DIGEST page 1401\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE ridge 6\n"
      "RENEWTICKET 0 5\n"
      "CHECKTICKET 0\n"
      "SAMPLE all 5\n"
      "FOLD\n"
      "LEDGER all\n"
      "FOLDLEDGER\n"
      "QUOTA 8\n"
      "ENFORCE documents\n"
      "LINKS 14 28 42\n"
      "RELATE mixed\n"
      "WALKREL 0 3\n"
      "BLOOM 7 2\n"
      "PROBE ridge\n"
      "INDEX 2 2\n"
      "SEEK 918281\n"
      "PLAN mixed 7\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 1401\n"
      "TOKENS 2\n"
      "PACK 8\n"
      "UNPACK\n"
      "ROLE ridge\n"
      "GRANT ridge read\n"
      "CHECK ridge read\n"
      "TIME 140 14000\n"
      "WINDOW 13900 14100\n"
      "RETAIN 1\n"
      "SHARD 4\n"
      "SHARDINFO\n"
      "INSPECT catalog\n",
      "story suite m workflow");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "CHECKPOINT 0\n",
      vesper::Status::limit,
      "story suite m checkpoint id limit");

  return vesper_test::finish(ctx);
}
