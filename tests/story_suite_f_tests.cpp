#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 701 seal-path 01020304\n"
      "PAGE 701 f0e0d0c0b0a09080\n"
      "SEGMENT 70 3\n"
      "ENTRY DOC ab\n"
      "ENTRY PAGE cd\n"
      "ENTRY META ef\n"
      "SEALSEG\n"
      "REPLAY 70\n"
      "SNAPSHOT 70\n"
      "DIGEST events 0\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE seal 8\n"
      "RENEWTICKET 0 2\n"
      "CHECKTICKET 0\n"
      "SAMPLE all 5\n"
      "FOLD\n"
      "LEDGER archive\n"
      "FOLDLEDGER\n"
      "QUOTA 7\n"
      "ENFORCE events\n"
      "RELATE mixed\n"
      "WALKREL 0 2\n"
      "BLOOM 5 2\n"
      "PROBE seal-path\n"
      "PLAN mixed 8\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 701\n"
      "TOKENS 3\n"
      "PACK 8\n"
      "UNPACK\n"
      "ROLE seal\n"
      "GRANT seal inspect\n"
      "CHECK seal inspect\n"
      "TIME 70 7000\n"
      "WINDOW 6900 7100\n"
      "RETAIN 1\n"
      "SHARD 4\n"
      "REBALANCE 16\n"
      "SHARDINFO\n",
      "story suite f workflow 1");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "DIGEST doc 999\n",
      vesper::Status::not_found,
      "story suite f missing digest source");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "LEDGER odd\n",
      vesper::Status::unsupported,
      "story suite f unknown ledger mode");

  return vesper_test::finish(ctx);
}
