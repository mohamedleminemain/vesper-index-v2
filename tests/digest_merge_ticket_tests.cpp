#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 alpha 01020304\n"
      "PAGE 1 aabbccdd\n"
      "DIGEST doc 1\n"
      "DIGEST page 1\n"
      "VERIFY 1\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE analyst 4\n"
      "CHECKTICKET 0\n",
      "digest merge ticket flow");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 2 beta 02030405\n"
      "PAGE 2 11223344\n"
      "MERGE mixed\n"
      "NEXT 1\n"
      "NEXT 1\n",
      "merge can advance in steps");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 3 gamma 0a0b0c0d\n"
      "ISSUE reader 8\n"
      "RENEWTICKET 0 5\n"
      "CHECKTICKET 0\n",
      "ticket issue renew check");

  vesper_test::expect_status(
      ctx,
      "VSP1\nVERIFY 7\n",
      vesper::Status::invalid_state,
      "verify requires captured digest");

  vesper_test::expect_status(
      ctx,
      "VSP1\nMERGE docs\nNEXT 1\n",
      vesper::Status::invalid_state,
      "merge on empty source is rejected");

  vesper_test::expect_status(
      ctx,
      "VSP1\nISSUE BAD 4\n",
      vesper::Status::limit,
      "ticket subject must be normalized");

  vesper_test::expect_status(
      ctx,
      "VSP1\nCHECKTICKET 0\n",
      vesper::Status::limit,
      "check ticket validates index");

  vesper_test::expect_status(
      ctx,
      "VSP1\nDOC 5 x 00\nMERGE strange\n",
      vesper::Status::unsupported,
      "merge rejects unknown mode");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 6 delta 0f0e0d0c\n"
      "PAGE 6 ddeeff00\n"
      "DIGEST doc 6\n"
      "DIGEST page 6\n"
      "MERGE mixed\n"
      "NEXT 2\n"
      "ISSUE auditor 6\n"
      "RENEWTICKET 0 3\n"
      "CHECKTICKET 0\n",
      "digest merge ticket digest path");

  return vesper_test::finish(ctx);
}
