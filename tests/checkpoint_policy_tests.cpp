#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 alpha 0102\n"
      "PAGE 1 aabbccdd\n"
      "CHECKPOINT 1\n"
      "CHECKVIEW 1\n"
      "POLICY documents max 4\n"
      "POLICY pages max 4\n"
      "POLICYCHECK documents max\n"
      "POLICYCHECK pages max\n",
      "checkpoint and policy workflow");

  vesper_test::expect_status(
      ctx,
      "VSP1\nCHECKVIEW 1\n",
      vesper::Status::not_found,
      "missing checkpoint rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nPOLICYCHECK documents max\n",
      vesper::Status::not_found,
      "missing policy rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nPOLICY documents max 0\n",
      vesper::Status::limit,
      "zero threshold rejected");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 2 beta 0203\n"
      "PAGE 2 ddeeff00\n"
      "CHECKPOINT 2\n"
      "CHECKVIEW 2\n"
      "POLICY documents max 5\n"
      "POLICYCHECK documents max\n",
      "checkpoint policy digest");

  return vesper_test::finish(ctx);
}
