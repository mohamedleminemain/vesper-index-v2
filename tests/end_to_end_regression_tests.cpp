#include "test_support.hpp"

#include <vector>

namespace {

struct ScriptCase {
  const char* name;
  vesper::Status status;
  const char* script;
};

}  // namespace

int main() {
  vesper_test::Context ctx;
  const std::vector<ScriptCase> cases{
      {"mixed-01", vesper::Status::ok,
       "VSP1\n"
       "DOC 1 atlas 01020304\n"
       "PAGE 1 aabbccdd\n"
       "SEGMENT 1 2\n"
       "ENTRY DOC 0102\n"
       "ENTRY PAGE aabb\n"
       "SEALSEG\n"
       "REPLAY 1\n"
       "BLOOM 4 2\n"
       "PROBE atlas\n"
       "PLAN docs 8\n"
       "EVAL\n"},
      {"mixed-02", vesper::Status::ok,
       "VSP1\n"
       "DOC 2 beta 11121314\n"
       "PAGE 2 101112131415\n"
       "SNAPSHOT 1\n"
       "RESTORE 1\n"
       "CACHE 2 4\n"
       "TOUCH 0\n"
       "METRICS all\n"
       "REDUCE\n"},
      {"mixed-03", vesper::Status::ok,
       "VSP1\n"
       "DOC 3 gamma 20212223\n"
       "NORMALIZE 3\n"
       "TOKENS 4\n"
       "PAGE 3 aaaaaabbbb\n"
       "PACK 4\n"
       "UNPACK\n"
       "INSPECT storage\n"},
      {"mixed-04", vesper::Status::ok,
       "VSP1\n"
       "ROLE analyst\n"
       "GRANT analyst read\n"
       "CHECK analyst read\n"
       "TIME 1 100\n"
       "WINDOW 50 150\n"
       "RETAIN 1\n"
       "SIGPLAN 1 1\n"
       "FINALIZE\n"
       "SIGN\n"},
      {"mixed-05", vesper::Status::ok,
       "VSP1\n"
       "DOC 4 delta 30313233\n"
       "PAGE 4 000102030405\n"
       "INDEX 2 2\n"
       "SEEK 262144\n"
       "SHARD 4\n"
       "REBALANCE 16\n"
       "SHARDINFO\n"},
      {"mixed-06", vesper::Status::ok,
       "VSP1\n"
       "DOC 5 epsilon 40414243\n"
       "VIEW 5\n"
       "MANIFEST\n"
       "SNAPSHOT 2\n"
       "BLOOM 5 3\n"
       "PROBE epsilon\n"
       "INSPECT catalog\n"},
      {"mixed-07", vesper::Status::ok,
       "VSP1\n"
       "DOC 6 zeta 50515253\n"
       "PAGE 6 998877665544\n"
       "CACHE 6 6\n"
       "PACK 3\n"
       "UNPACK\n"
       "METRICS pages\n"
       "REDUCE\n"},
      {"mixed-08", vesper::Status::ok,
       "VSP1\n"
       "DOC 7 eta 60616263\n"
       "LINKS 1 3 5 7\n"
       "FOLLOW 2\n"
       "EXPORT\n"
       "BLOOM 4 2\n"
       "PROBE link:5\n"
       "PLAN graph 32\n"
       "EVAL\n"},
      {"mixed-09", vesper::Status::ok,
       "VSP1\n"
       "DOC 8 theta 70717273\n"
       "SCHEMA 4 a 0 2 b 2 2\n"
       "ROW 01020304\n"
       "PROJECT\n"
       "METRICS docs\n"
       "REDUCE\n"
       "INSPECT query\n"},
      {"mixed-10", vesper::Status::ok,
       "VSP1\n"
       "DOC 9 iota 80818283\n"
       "NORMALIZE 9\n"
       "TOKENS 8\n"
       "ROLE reviewer\n"
       "GRANT reviewer read\n"
       "CHECK reviewer write\n"
       "TIME 2 200\n"
       "WINDOW 100 250\n"},
      {"mixed-11", vesper::Status::ok,
       "VSP1\n"
       "PAGE 10 aabbccddeeff\n"
       "CURSOR 10 1\n"
       "SCAN 0\n"
       "INDEX 1 3\n"
       "SEEK 65546\n"
       "CACHE 10 4\n"
       "TOUCH 0\n"},
      {"mixed-12", vesper::Status::ok,
       "VSP1\n"
       "PAGE 11 010101010101\n"
       "PACK 6\n"
       "UNPACK\n"
       "PALETTE 3\n"
       "COLOR\n"
       "LABEL 5 north\n"
       "RENDER\n"},
      {"mixed-13", vesper::Status::ok,
       "VSP1\n"
       "KEY 1 00112233\n"
       "SIGPLAN 2 2\n"
       "FINALIZE\n"
       "SIGN\n"
       "ROLE signer\n"
       "GRANT signer approve\n"
       "CHECK signer approve\n"},
      {"mixed-14", vesper::Status::ok,
       "VSP1\n"
       "DOC 12 lambda 12121212\n"
       "PAGE 12 a0a1a2a3a4a5\n"
       "SEGMENT 4 2\n"
       "ENTRY DOC aaaa\n"
       "ENTRY PAGE bbbb\n"
       "SEALSEG\n"
       "SNAPSHOT 3\n"
       "RESTORE 3\n"},
      {"mixed-15", vesper::Status::ok,
       "VSP1\n"
       "DOC 13 mu 13131313\n"
       "BLOOM 6 2\n"
       "PROBE mu\n"
       "NORMALIZE 13\n"
       "TOKENS 2\n"
       "PLAN mixed 24\n"
       "EVAL\n"},
      {"mixed-16", vesper::Status::ok,
       "VSP1\n"
       "DOC 14 nu 14141414\n"
       "PAGE 14 141516171819\n"
       "SHARD 3\n"
       "REBALANCE 9\n"
       "SHARDINFO\n"
       "INSPECT storage\n"},
      {"mixed-17", vesper::Status::ok,
       "VSP1\n"
       "DOC 15 xi 15151515\n"
       "PAGE 15 ffeeddccbbaa\n"
       "SNAPSHOT 4\n"
       "CACHE 15 3\n"
       "TOUCH 0\n"
       "RESTORE 4\n"},
      {"mixed-18", vesper::Status::ok,
       "VSP1\n"
       "DOC 16 omicron 16161616\n"
       "PAGE 16 0f0e0d0c0b0a\n"
       "BLOOM 8 3\n"
       "INDEX 2 3\n"
       "PLAN mixed 40\n"
       "EVAL\n"
       "METRICS all\n"
       "REDUCE\n"},
      {"mixed-19", vesper::Status::ok,
       "VSP1\n"
       "DOC 17 pi 17171717\n"
       "NORMALIZE 17\n"
       "TOKENS 3\n"
       "TIME 4 400\n"
       "WINDOW 350 450\n"
       "RETAIN 1\n"},
      {"mixed-20", vesper::Status::ok,
       "VSP1\n"
       "DOC 18 rho 18181818\n"
       "PAGE 18 001100110011\n"
       "PACK 4\n"
       "UNPACK\n"
       "SEGMENT 5 1\n"
       "ENTRY META abcd\n"
       "SEALSEG\n"
       "REPLAY 5\n"},
      {"mixed-21", vesper::Status::ok,
       "VSP1\n"
       "DOC 19 sigma 19191919\n"
       "BLOOM 4 2\n"
       "PROBE sigma\n"
       "INSPECT query\n"},
      {"mixed-22", vesper::Status::ok,
       "VSP1\n"
       "DOC 20 tau 20202020\n"
       "PAGE 20 1234567890ab\n"
       "CACHE 20 5\n"
       "EVICT 32\n"
       "INSPECT storage\n"},
      {"mixed-23", vesper::Status::ok,
       "VSP1\n"
       "ROLE operator\n"
       "GRANT operator read\n"
       "GRANT operator write\n"
       "CHECK operator read\n"
       "CHECK operator execute\n"
       "INSPECT security\n"},
      {"mixed-24", vesper::Status::ok,
       "VSP1\n"
       "PAGE 21 aa00bb00cc00\n"
       "INDEX 3 2\n"
       "SEEK 1376256\n"
       "METRICS pages\n"
       "REDUCE\n"},
      {"mixed-25", vesper::Status::ok,
       "VSP1\n"
       "DOC 22 upsilon 22222222\n"
       "LINKS 2 4 6 8\n"
       "FOLLOW 0\n"
       "EXPORT\n"
       "PLAN graph 12\n"
       "EVAL\n"},
      {"mixed-26", vesper::Status::ok,
       "VSP1\n"
       "DOC 23 phi 23232323\n"
       "NORMALIZE 23\n"
       "TOKENS 5\n"
       "PALETTE 2\n"
       "COLOR\n"},
      {"mixed-27", vesper::Status::ok,
       "VSP1\n"
       "DOC 24 chi 24242424\n"
       "SNAPSHOT 5\n"
       "RESTORE 5\n"
       "SNAPINFO\n"},
      {"mixed-28", vesper::Status::ok,
       "VSP1\n"
       "DOC 25 psi 25252525\n"
       "PAGE 25 abcdef012345\n"
       "PACK 6\n"
       "UNPACK\n"
       "METRICS all\n"
       "REDUCE\n"},
      {"mixed-29", vesper::Status::ok,
       "VSP1\n"
       "DOC 26 omega 26262626\n"
       "BLOOM 4 2\n"
       "PROBE omega\n"
       "PROBE absent\n"
       "INSPECT query\n"},
      {"mixed-30", vesper::Status::ok,
       "VSP1\n"
       "ROLE reader\n"
       "GRANT reader read\n"
       "CHECK reader read\n"
       "TIME 7 700\n"
       "WINDOW 650 750\n"
       "RETAIN 1\n"
       "INSPECT security\n"},
      {"mixed-31", vesper::Status::ok,
       "VSP1\n"
       "DOC 27 mix 27272727\n"
       "PAGE 27 999988887777\n"
       "SHARD 5\n"
       "SHARDINFO\n"
       "PLAN docs 11\n"
       "EVAL\n"},
      {"mixed-32", vesper::Status::ok,
       "VSP1\n"
       "DOC 28 blend 28282828\n"
       "SNAPSHOT 6\n"
       "SEGMENT 6 1\n"
       "ENTRY META feed\n"
       "SEALSEG\n"
       "REPLAY 6\n"
       "RESTORE 6\n"},
      {"mixed-33", vesper::Status::ok,
       "VSP1\n"
       "PAGE 29 0102030405060708\n"
       "CACHE 29 6\n"
       "PACK 8\n"
       "UNPACK\n"
       "INDEX 2 2\n"
       "SEEK 1900800\n"},
      {"mixed-34", vesper::Status::ok,
       "VSP1\n"
       "DOC 30 north-west 30303030\n"
       "NORMALIZE 30\n"
       "TOKENS 8\n"
       "BLOOM 8 3\n"
       "PROBE north-west\n"},
      {"mixed-35", vesper::Status::ok,
       "VSP1\n"
       "DOC 31 script 31313131\n"
       "PAGE 31 abababababab\n"
       "METRICS all\n"
       "REDUCE\n"
       "INSPECT catalog\n"},
      {"mixed-36", vesper::Status::ok,
       "VSP1\n"
       "DOC 32 frame 32323232\n"
       "PAGE 32 cdcdcdcdcdcd\n"
       "CACHE 32 4\n"
       "TOUCH 0\n"
       "PACK 2\n"
       "UNPACK\n"},
      {"mixed-37", vesper::Status::ok,
       "VSP1\n"
       "ROLE guard\n"
       "GRANT guard read\n"
       "GRANT guard inspect\n"
       "CHECK guard inspect\n"
       "SIGPLAN 2 2\n"
       "FINALIZE\n"
       "SIGN\n"},
      {"mixed-38", vesper::Status::ok,
       "VSP1\n"
       "DOC 33 lane 33333333\n"
       "LINKS 10 20 30\n"
       "FOLLOW 1\n"
       "EXPORT\n"
       "BLOOM 6 2\n"
       "PROBE link:20\n"},
      {"mixed-39", vesper::Status::ok,
       "VSP1\n"
       "PAGE 34 deadbeefcafe\n"
       "INDEX 2 4\n"
       "SEEK 2228258\n"
       "CACHE 34 3\n"
       "EVICT 20\n"},
      {"mixed-40", vesper::Status::ok,
       "VSP1\n"
       "DOC 35 orbit 35353535\n"
       "NORMALIZE 35\n"
       "TOKENS 2\n"
       "INSPECT query\n"},
      {"mixed-41", vesper::Status::limit,
       "VSP1\n"
       "BLOOM 1 1\n"},
      {"mixed-42", vesper::Status::invalid_state,
       "VSP1\n"
       "SEALSEG\n"},
      {"mixed-43", vesper::Status::not_found,
       "VSP1\n"
       "RESTORE 99\n"},
      {"mixed-44", vesper::Status::limit,
       "VSP1\n"
       "PACK 0\n"},
      {"mixed-45", vesper::Status::invalid_state,
       "VSP1\n"
       "UNPACK\n"},
      {"mixed-46", vesper::Status::limit,
       "VSP1\n"
       "ROLE BAD\n"},
      {"mixed-47", vesper::Status::malformed,
       "VSP1\n"
       "WINDOW 20 10\n"},
      {"mixed-48", vesper::Status::unsupported,
       "VSP1\n"
       "INSPECT ghost\n"},
      {"mixed-49", vesper::Status::invalid_state,
       "VSP1\n"
       "TOKENS 1\n"},
      {"mixed-50", vesper::Status::invalid_state,
       "VSP1\n"
       "EVAL\n"}};

  for (const auto& test : cases) {
    const auto result = vesper_test::run_script(test.script);
    ctx.expect(result.status == test.status, test.name);
  }

  return vesper_test::finish(ctx);
}
