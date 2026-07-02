#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 1 atlas 01020304\n"
      "VIEW 1\n"
      "MANIFEST\n"
      "PAGE 1 aabbccddeeff\n"
      "CURSOR 1 2\n"
      "SCAN 0\n"
      "SEGMENT 1 2\n"
      "ENTRY DOC aa\n"
      "ENTRY PAGE bb\n"
      "SEALSEG\n"
      "REPLAY 1\n"
      "SNAPSHOT 1\n"
      "CACHE 1 4\n"
      "TOUCH 0\n"
      "BLOOM 8 3\n"
      "PROBE atlas\n"
      "INDEX 2 3\n"
      "SEEK 65537\n"
      "PLAN mixed 32\n"
      "EVAL\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 1\n"
      "TOKENS 4\n"
      "PACK 6\n"
      "UNPACK\n"
      "ROLE analyst\n"
      "GRANT analyst read\n"
      "CHECK analyst read\n"
      "TIME 1 100\n"
      "WINDOW 50 150\n"
      "RETAIN 1\n"
      "INSPECT catalog\n",
      "full archive-query-codec-session path one");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 2 beta 11121314\n"
      "DOC 3 gamma 21222324\n"
      "PAGE 2 001122334455\n"
      "PAGE 3 66778899aabb\n"
      "SNAPSHOT 2\n"
      "RESTORE 2\n"
      "CACHE 2 3\n"
      "CACHE 3 4\n"
      "EVICT 12\n"
      "LINKS 3 5 8 13\n"
      "FOLLOW 2\n"
      "EXPORT\n"
      "NODE 5 500\n"
      "WATCH 5\n"
      "AUDIT\n"
      "BLOOM 10 4\n"
      "PROBE beta\n"
      "PLAN graph 64\n"
      "EVAL\n"
      "NORMALIZE 2\n"
      "TOKENS 3\n"
      "ROLE writer\n"
      "GRANT writer read\n"
      "GRANT writer write\n"
      "CHECK writer write\n"
      "TIME 2 200\n"
      "TIME 3 300\n"
      "WINDOW 150 350\n"
      "RETAIN 2\n"
      "SHARD 4\n"
      "REBALANCE 18\n"
      "SHARDINFO\n",
      "full path with links and sharding");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 4 delta 31323334\n"
      "PAGE 4 a1a2a3a4a5a6\n"
      "SEGMENT 4 3\n"
      "ENTRY DOC 0101\n"
      "ENTRY PAGE 0202\n"
      "ENTRY META 0303\n"
      "SEALSEG\n"
      "REPLAY 4\n"
      "SNAPSHOT 4\n"
      "RESTORE 4\n"
      "SCHEMA 4 a 0 2 b 2 2\n"
      "ROW 01020304\n"
      "PROJECT\n"
      "METRICS docs\n"
      "REDUCE\n"
      "NORMALIZE 4\n"
      "TOKENS 5\n"
      "PACK 4\n"
      "UNPACK\n"
      "PALETTE 4\n"
      "COLOR\n"
      "SIGPLAN 2 2\n"
      "FINALIZE\n"
      "SIGN\n",
      "full path with snapshot schema and session");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 5 epsilon 41424344\n"
      "VIEW 5\n"
      "RENAME 5 broad\n"
      "MANIFEST\n"
      "PAGE 5 ff00ee11dd22\n"
      "CURSOR 5 1\n"
      "SCAN 0\n"
      "COMPACT\n"
      "CACHE 5 5\n"
      "TOUCH 0\n"
      "INDEX 1 4\n"
      "SEEK 327685\n"
      "PLAN pages 16\n"
      "EVAL\n"
      "ROLE reviewer\n"
      "GRANT reviewer read\n"
      "CHECK reviewer write\n"
      "TIME 6 600\n"
      "WINDOW 500 700\n"
      "RETAIN 1\n"
      "INSPECT storage\n",
      "page-heavy integration path");

  vesper_test::expect_ok(
      ctx,
      "VSP1\n"
      "DOC 6 zeta 51525354\n"
      "PAGE 6 102030405060\n"
      "SEGMENT 6 1\n"
      "ENTRY META abcd\n"
      "SEALSEG\n"
      "REPLAY 6\n"
      "BLOOM 7 3\n"
      "PROBE zeta\n"
      "METRICS all\n"
      "REDUCE\n"
      "NORMALIZE 6\n"
      "TOKENS 2\n"
      "PACK 3\n"
      "UNPACK\n"
      "ROLE operator\n"
      "GRANT operator inspect\n"
      "CHECK operator inspect\n"
      "SHARD 3\n"
      "SHARDINFO\n",
      "integration path with compact feature set");

  vesper_test::expect_digest(
      ctx,
      "VSP1\n"
      "DOC 7 eta 61626364\n"
      "PAGE 7 7766554433221100\n"
      "SNAPSHOT 7\n"
      "CACHE 7 6\n"
      "BLOOM 8 2\n"
      "PROBE eta\n"
      "INDEX 2 2\n"
      "SEEK 458759\n"
      "PLAN mixed 20\n"
      "EVAL\n"
      "NORMALIZE 7\n"
      "TOKENS 8\n"
      "PACK 8\n"
      "UNPACK\n"
      "ROLE guard\n"
      "GRANT guard inspect\n"
      "CHECK guard inspect\n"
      "TIME 7 700\n"
      "WINDOW 650 750\n"
      "RETAIN 1\n"
      "INSPECT security\n",
      "integration digest path");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "DOC 8 theta 71727374\n"
      "BLOOM 1 1\n",
      vesper::Status::limit,
      "integration still reports bloom parameter failure");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "PAGE 8 aa\n"
      "PACK 0\n",
      vesper::Status::limit,
      "integration still reports pack limit failure");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "ROLE bad\n"
      "GRANT bad read\n"
      "GRANT bad read\n",
      vesper::Status::invalid_state,
      "integration still reports duplicate grant failure");

  vesper_test::expect_status(
      ctx,
      "VSP1\n"
      "TIME 9 900\n"
      "WINDOW 950 850\n",
      vesper::Status::malformed,
      "integration still reports window ordering failure");

  return vesper_test::finish(ctx);
}
