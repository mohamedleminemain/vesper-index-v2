#include "test_support.hpp"

namespace {
constexpr const char* kPassScript =
    "VSP1\nDOC 1 alpha 00\nPOLICY documents max 4\nPOLICYCHECK documents max\n";
}

int main() {
  vesper_test::Context ctx;
  vesper_test::expect_ok(
      ctx,
      kPassScript,
      "policy smoke pass");
  vesper_test::expect_status(
      ctx,
      "VSP1\nPOLICYCHECK documents max\n",
      vesper::Status::not_found,
      "policy smoke missing rule");
  return vesper_test::finish(ctx);
}
