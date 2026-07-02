#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 atlas 010203\n"
      "PAGE 9 aabbccddee\n"
      "SCHEMA 4 unit 0 2 pair 2 2\n"
      "ROW 01020304\n"
      "LINKS 2 4 6\n"
      "NODE 7 99\n"
      "WATCH 7\n"
      "BLOOM 4 3\n"
      "PROBE atlas\n"
      "INDEX 2 3\n"
      "SEEK 65537\n"
      "PLAN mixed 32\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "INSPECT query\n",
      "query subsystems cooperate");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "BLOOM 1 1\n",
      vesper::Status::limit,
      "bloom requires at least two words");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "PROBE hello\n",
      vesper::Status::invalid_state,
      "probe requires prepared bloom filter");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "INDEX 0 1\n",
      vesper::Status::limit,
      "skip index rejects zero stride");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "SEEK 12\n",
      vesper::Status::invalid_state,
      "seek requires skip index");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "PLAN docs 0\n",
      vesper::Status::limit,
      "plan limit must be non-zero");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "EVAL\n",
      vesper::Status::invalid_state,
      "cannot evaluate absent plan");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "METRICS weird\n",
      vesper::Status::unsupported,
      "metrics reject unknown scope");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "REDUCE\n",
      vesper::Status::invalid_state,
      "cannot reduce before metrics are collected");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "INSPECT mystery\n",
      vesper::Status::unsupported,
      "inspect view must be known");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 10 north 01020304\n"
      "DOC 11 south 05060708\n"
      "PAGE 1 aabbccddeeff\n"
      "PAGE 2 001122334455\n"
      "BLOOM 8 4\n"
      "PROBE north\n"
      "PROBE west\n",
      "bloom distinguishes hit and miss");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 3 aabbccdd\n"
      "PAGE 4 10203040\n"
      "INDEX 1 4\n"
      "SEEK 196609\n",
      "skip index can be built on pages");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 4 delta 0a0b0c0d\n"
      "PAGE 5 010203040506\n"
      "LINKS 1 9 27 81\n"
      "NODE 1 101\n"
      "WATCH 1\n"
      "PLAN graph 64\n"
      "EVAL\n",
      "graph plan path works");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 a 01\n"
      "DOC 2 bb 0202\n"
      "DOC 3 ccc 030303\n"
      "METRICS docs\n"
      "REDUCE\n",
      "document metrics path works");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 11 00010203\n"
      "PAGE 12 0405060708\n"
      "METRICS pages\n"
      "REDUCE\n",
      "page metrics path works");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "LINKS 8 13 21 34\n"
      "NODE 4 64\n"
      "WATCH 4\n"
      "METRICS graph\n"
      "REDUCE\n",
      "graph metrics path works");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 origin 0101\n"
      "PAGE 6 0011223344556677\n"
      "BLOOM 16 3\n"
      "INDEX 2 2\n"
      "PLAN mixed 128\n"
      "EVAL\n"
      "INSPECT query\n",
      "plan can leverage bloom and skip subsystems");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 alpha 0102\n"
      "DOC 2 beta 0304\n"
      "PAGE 8 aabbccdd\n"
      "BLOOM 6 2\n"
      "PROBE alpha\n"
      "PROBE beta\n"
      "PROBE gamma\n",
      "multiple probes are accepted");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 13 1112131415161718\n"
      "INDEX 3 4\n"
      "SEEK 851981\n"
      "SEEK 917517\n",
      "skip index can be reused for repeated seeks");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 7 archive 00112233\n"
      "PAGE 14 445566778899\n"
      "BLOOM 5 3\n"
      "PLAN docs 16\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "INSPECT query\n",
      "query inspection can follow planning and metrics");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 9 paper 0102030405\n"
      "PAGE 18 aa55aa55aa55\n"
      "SCHEMA 4 x 0 1 y 1 1 z 2 2\n"
      "ROW 01020304\n"
      "PROJECT\n"
      "BLOOM 7 3\n"
      "INDEX 2 3\n"
      "PLAN mixed 24\n"
      "EVAL\n",
      "query subsystems coexist with existing row projection");

  return vesper_test::finish(ctx);
}
