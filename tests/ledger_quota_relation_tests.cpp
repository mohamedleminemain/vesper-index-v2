#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;
  vesper_test::expect_ok(
      ctx,
      "VSP1\nDOC 1 alpha 0102\nPAGE 1 aabbccdd\nLEDGER all\nFOLDLEDGER\nQUOTA 4\nENFORCE documents\nLINKS 2 4 6\nRELATE mixed\nWALKREL 0 2\n",
      "ledger quota relation workflow");
  vesper_test::expect_status(
      ctx,
      "VSP1\nFOLDLEDGER\n",
      vesper::Status::invalid_state,
      "ledger fold requires entries");
  vesper_test::expect_status(
      ctx,
      "VSP1\nENFORCE documents\n",
      vesper::Status::invalid_state,
      "quota must be prepared");
  vesper_test::expect_status(
      ctx,
      "VSP1\nRELATE mixed\n",
      vesper::Status::invalid_state,
      "relation requires source state");
  vesper_test::expect_digest(
      ctx,
      "VSP1\nDOC 2 beta 0304\nPAGE 2 11223344\nNODE 1 9\nLINKS 5 7 9\nLEDGER query\nFOLDLEDGER\nQUOTA 3\nENFORCE events\nRELATE mixed\nWALKREL 0 3\n",
      "ledger quota relation digest");
  return vesper_test::finish(ctx);
}
