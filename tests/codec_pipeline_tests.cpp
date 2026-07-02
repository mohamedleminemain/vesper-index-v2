#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 North-West 01020304\n"
      "NORMALIZE 1\n"
      "TOKENS 4\n"
      "PAGE 9 aaaaaabbbbcccc\n"
      "PACK 6\n"
      "UNPACK\n"
      "VECTOR 8 2 aabb\n"
      "DECODE\n"
      "LABEL 5 north\n"
      "RENDER\n"
      "PALETTE 7\n"
      "COLOR\n",
      "codec workflows compose");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "NORMALIZE 1\n",
      vesper::Status::not_found,
      "normalizer requires known document");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "TOKENS 1\n",
      vesper::Status::invalid_state,
      "token emission requires prepared normalizer");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "PACK 0\n",
      vesper::Status::limit,
      "pack rejects zero block size");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "UNPACK\n",
      vesper::Status::invalid_state,
      "unpack requires packed blocks");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 10 alpha_beta 01020304\n"
      "NORMALIZE 10\n"
      "TOKENS 8\n",
      "normalizer tokenizes punctuation");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 1 aaaaaaaaaa\n"
      "PAGE 2 bbbbccccdddd\n"
      "PACK 5\n"
      "UNPACK\n",
      "block packing roundtrips multiple pages");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 4 East-West 0a0b0c0d\n"
      "NORMALIZE 4\n"
      "TOKENS 2\n"
      "PAGE 7 001100110011\n"
      "PACK 3\n"
      "UNPACK\n"
      "PALETTE 1\n"
      "COLOR\n",
      "new codec subsystems coexist with palette output");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 6 Spiral-Arm 11223344\n"
      "NORMALIZE 6\n"
      "TOKENS 10\n"
      "LABEL 5 north\n"
      "RENDER\n",
      "normalizer and label renderer work together");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "PAGE 4 aa55aa55aa55aa55\n"
      "PACK 4\n"
      "UNPACK\n"
      "VECTOR 4 2 1122\n"
      "DECODE\n",
      "blocks coexist with typed vector decode");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "DOC 8 north 00\n"
      "NORMALIZE 8\n"
      "TOKENS 0\n",
      vesper::Status::limit,
      "token emission enforces positive limit");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 12 x-y_z 01010101\n"
      "NORMALIZE 12\n"
      "TOKENS 16\n"
      "PAGE 8 aabbccaabbcc\n"
      "PACK 8\n"
      "UNPACK\n",
      "normalized tokens can be emitted after packing blocks");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 14 Orion Belt 01234567\n"
      "NORMALIZE 14\n"
      "TOKENS 4\n"
      "PAGE 19 9988776655443322\n"
      "PACK 7\n"
      "UNPACK\n"
      "VECTOR 6 2 aabbcc\n"
      "DECODE\n"
      "LABEL 5 north\n"
      "RENDER\n"
      "PALETTE 5\n"
      "COLOR\n",
      "extended codec workflow yields digest");

  return vesper_test::finish(ctx);
}
