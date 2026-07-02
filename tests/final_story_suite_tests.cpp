#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1601 summit-path 010203040506\n"
      "VIEW 1601\n"
      "PAGE 1601 aabbccddeeff00112233\n"
      "CURSOR 1601 3\n"
      "SCAN 0\n"
      "SEGMENT 160 2\n"
      "ENTRY DOC a1a2\n"
      "ENTRY PAGE b1b2\n"
      "SEALSEG\n"
      "REPLAY 160\n"
      "SNAPSHOT 160\n"
      "CHECKPOINT 160\n"
      "CHECKVIEW 160\n"
      "POLICY documents max 10\n"
      "POLICY pages max 10\n"
      "POLICY events max 128\n"
      "POLICYCHECK documents max\n"
      "POLICYCHECK pages max\n"
      "POLICYCHECK events max\n"
      "DIGEST doc 1601\n"
      "DIGEST page 1601\n"
      "DIGEST events 0\n"
      "MERGE mixed\n"
      "NEXT 3\n"
      "ISSUE summit 7\n"
      "RENEWTICKET 0 5\n"
      "CHECKTICKET 0\n"
      "SAMPLE all 6\n"
      "FOLD\n"
      "LEDGER all\n"
      "FOLDLEDGER\n"
      "QUOTA 9\n"
      "ENFORCE documents\n"
      "ENFORCE pages\n"
      "LINKS 16 32 48 64\n"
      "FOLLOW 2\n"
      "EXPORT\n"
      "RELATE mixed\n"
      "WALKREL 0 4\n"
      "BLOOM 8 3\n"
      "PROBE summit-path\n"
      "INDEX 2 3\n"
      "SEEK 1049025\n"
      "PLAN mixed 14\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 1601\n"
      "TOKENS 4\n"
      "PACK 8\n"
      "UNPACK\n"
      "PALETTE 5\n"
      "COLOR\n"
      "LABEL 5 north\n"
      "RENDER\n"
      "ROLE summit\n"
      "GRANT summit read\n"
      "GRANT summit inspect\n"
      "CHECK summit read\n"
      "CHECK summit inspect\n"
      "TIME 160 16000\n"
      "WINDOW 15900 16100\n"
      "RETAIN 1\n"
      "SHARD 5\n"
      "REBALANCE 15\n"
      "SHARDINFO\n"
      "INSPECT catalog\n"
      "INSPECT storage\n"
      "INSPECT query\n"
      "INSPECT security\n",
      "final story suite workflow");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "CHECKVIEW 999\n",
      vesper::Status::not_found,
      "final story suite missing checkpoint");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "POLICY docs max 0\n",
      vesper::Status::limit,
      "final story suite invalid policy threshold");

  return vesper_test::finish(ctx);
}
