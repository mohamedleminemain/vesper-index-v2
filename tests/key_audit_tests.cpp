#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "KEY 7 0011223344556677\n"
      "KEYLEASE 11 7 ingest\n"
      "KEYAUDIT active\n"
      "KEYRELEASE 11\n"
      "KEYAUDIT history\n",
      "key audit workflow");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEYLEASE 1 7 ingest\n",
      vesper::Status::not_found,
      "missing key rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 7 0011223344556677\nKEYLEASE 1 7 ingest\nKEYLEASE 1 7 export\n",
      vesper::Status::invalid_state,
      "duplicate lease rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 7 0011223344556677\nKEYLEASE 1 7 ingest\nKEYRELEASE 2\n",
      vesper::Status::not_found,
      "missing lease release rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nKEY 7 0011223344556677\nKEYAUDIT active\n",
      vesper::Status::invalid_state,
      "empty audit rejected");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "KEY 3 aabbccddeeff0011\n"
      "KEYLEASE 4 3 archive\n"
      "KEYAUDIT leases\n",
      "key audit digest");

  return vesper_test::finish(ctx);
}
