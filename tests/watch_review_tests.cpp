#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "NODE 7 99\n"
      "NODE 8 120\n"
      "WATCH 7\n"
      "RELATE nodes\n"
      "WALKREL 7 1\n"
      "TIME 7 100\n"
      "WINDOW 0 200\n"
      "WATCHSNAP 7\n"
      "WATCHREVIEW all\n",
      "watch review workflow");

  vesper_test::expect_status(
      ctx,
      "VSP1\nWATCHSNAP 1\n",
      vesper::Status::invalid_state,
      "empty watch snapshot rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nNODE 7 99\nWATCH 7\nWATCHREVIEW summary\n",
      vesper::Status::invalid_state,
      "review before snapshot rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nNODE 7 99\nWATCH 7\nWATCHSNAP 7\nWATCHREVIEW mystery\n",
      vesper::Status::unsupported,
      "unknown review mode rejected");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "NODE 9 144\n"
      "WATCH 9\n"
      "TIME 9 500\n"
      "WINDOW 0 1000\n"
      "WATCHSNAP 9\n"
      "WATCHREVIEW timeline\n",
      "watch review digest");

  return vesper_test::finish(ctx);
}
