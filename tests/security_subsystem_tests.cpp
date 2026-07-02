#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "ROLE analyst\n"
      "GRANT analyst read\n"
      "GRANT analyst export\n"
      "CHECK analyst read\n"
      "CHECK analyst write\n"
      "TIME 1 100\n"
      "TIME 2 200\n"
      "WINDOW 50 150\n"
      "RETAIN 1\n"
      "SIGPLAN 2 2\n"
      "FINALIZE\n"
      "SIGN\n",
      "security and timeline flows compose");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "ROLE ANALYST\n",
      vesper::Status::limit,
      "role names are normalized");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "GRANT analyst read\n",
      vesper::Status::not_found,
      "grant requires existing role");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "ROLE analyst\n"
      "CHECK analyst read\n",
      vesper::Status::ok,
      "permission checks can deny without failing");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "TIME 0 10\n",
      vesper::Status::limit,
      "timeline tag zero is rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "WINDOW 20 10\n",
      vesper::Status::malformed,
      "timeline window validates ordering");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "RETAIN 0\n",
      vesper::Status::limit,
      "retain count must be positive");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "ROLE writer\n"
      "GRANT writer read\n"
      "GRANT writer write\n"
      "CHECK writer write\n"
      "CHECK writer export\n",
      "acl role can allow and deny different permissions");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "TIME 5 10\n"
      "TIME 6 20\n"
      "TIME 7 30\n"
      "WINDOW 10 20\n"
      "RETAIN 2\n",
      "timeline can window and retain events");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "ROLE reader\n"
      "GRANT reader read\n"
      "ROLE operator\n"
      "GRANT operator read\n"
      "GRANT operator write\n"
      "CHECK reader read\n"
      "CHECK operator write\n",
      "multiple roles coexist");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "TIME 1 100\n"
      "TIME 2 150\n"
      "TIME 3 175\n"
      "WINDOW 120 180\n"
      "RETAIN 2\n"
      "TIME 4 300\n",
      "timeline remains usable after retain");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "ROLE audit\n"
      "GRANT audit read\n"
      "GRANT audit inspect\n"
      "CHECK audit inspect\n"
      "TIME 4 400\n"
      "TIME 5 500\n"
      "WINDOW 350 550\n"
      "RETAIN 2\n"
      "SIGPLAN 3 3\n"
      "FINALIZE\n"
      "SIGN\n",
      "security workflow yields digest");

  return vesper_test::finish(ctx);
}
