#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 301 kappa 01010101\n"
      "PAGE 301 aa55aa55aa55\n"
      "CACHE 301 4\n"
      "TOUCH 0\n"
      "DIGEST page 301\n"
      "MERGE pages\n"
      "NEXT 1\n"
      "ISSUE ops 8\n"
      "CHECKTICKET 0\n"
      "SAMPLE pages 1\n"
      "FOLD\n"
      "LEDGER codec\n"
      "FOLDLEDGER\n"
      "QUOTA 9\n"
      "ENFORCE pages\n"
      "RELATE docs\n"
      "BLOOM 6 2\n"
      "PROBE kappa\n"
      "INDEX 2 2\n"
      "SEEK 1971661\n"
      "PLAN pages 8\n"
      "EVAL\n"
      "METRICS pages\n"
      "REDUCE\n"
      "NORMALIZE 301\n"
      "TOKENS 2\n"
      "PACK 4\n"
      "UNPACK\n"
      "PALETTE 3\n"
      "COLOR\n"
      "LABEL 5 north\n"
      "RENDER\n"
      "ROLE reviewer\n"
      "GRANT reviewer read\n"
      "CHECK reviewer read\n"
      "TIME 30 3000\n"
      "WINDOW 2900 3100\n"
      "RETAIN 1\n"
      "SHARD 5\n"
      "REBALANCE 10\n"
      "SHARDINFO\n",
      "story suite b workflow 1");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 302 lambda 02020202\n"
      "DOC 303 mu 03030303\n"
      "SNAPSHOT 31\n"
      "PAGE 302 11223344\n"
      "SNAPSHOT 32\n"
      "DIFFSNAP 31 32\n"
      "DIGEST events 0\n"
      "MERGE docs\n"
      "NEXT 2\n"
      "ISSUE auditor 5\n"
      "RENEWTICKET 0 4\n"
      "CHECKTICKET 0\n"
      "SAMPLE docs 2\n"
      "FOLD\n"
      "LEDGER archive\n"
      "FOLDLEDGER\n"
      "QUOTA 4\n"
      "ENFORCE documents\n"
      "RELATE mixed\n"
      "WALKREL 302 1\n"
      "INSPECT storage\n",
      "story suite b workflow 2");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "CHECKTICKET 0\n",
      vesper::Status::limit,
      "story suite b ticket index check");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "RELATE mixed\n",
      vesper::Status::invalid_state,
      "story suite b relation empty");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "SAMPLE docs 0\n",
      vesper::Status::limit,
      "story suite b sample limit");

  return vesper_test::finish(ctx);
}
