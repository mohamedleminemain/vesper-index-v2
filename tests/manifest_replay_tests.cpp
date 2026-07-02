#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"replay-a-01", vesper::Status::ok, "VSP1\nDOC 1 a 00\nMANIFEST\n"},
      {"replay-a-02", vesper::Status::ok, "VSP1\nDOC 2 b 0102\nVIEW 2\nMANIFEST\n"},
      {"replay-a-03", vesper::Status::ok, "VSP1\nPAGE 1 aabb\nCURSOR 1 0\nSCAN 0\n"},
      {"replay-a-04", vesper::Status::ok, "VSP1\nPAGE 2 aabbccdd\nCURSOR 2 3\nSCAN 0\n"},
      {"replay-a-05", vesper::Status::ok, "VSP1\nSEGMENT 1 1\nENTRY META aa\nSEALSEG\n"},
      {"replay-a-06", vesper::Status::ok, "VSP1\nSEGMENT 2 2\nENTRY DOC aa\nENTRY PAGE bb\nSEALSEG\nREPLAY 2\n"},
      {"replay-a-07", vesper::Status::ok, "VSP1\nDOC 3 c 03\nSNAPSHOT 1\nRESTORE 1\n"},
      {"replay-a-08", vesper::Status::ok, "VSP1\nPAGE 3 01020304\nCACHE 3 2\nTOUCH 0\n"},
      {"replay-a-09", vesper::Status::ok, "VSP1\nKEY 1 00112233\n"},
      {"replay-a-10", vesper::Status::ok, "VSP1\nKEY 1 00112233\nROTATE 1 2 44556677\n"},
      {"replay-a-11", vesper::Status::ok, "VSP1\nDOC 4 d 04\nPAGE 4 aabbcc\nSNAPSHOT 2\nSNAPINFO\n"},
      {"replay-a-12", vesper::Status::ok, "VSP1\nPAGE 5 0011223344\nCOMPACT\n"},
      {"replay-a-13", vesper::Status::ok, "VSP1\nDOC 5 e 05\nRENAME 5 east\nVIEW 5\n"},
      {"replay-a-14", vesper::Status::ok, "VSP1\nPAGE 6 abcdef\nCACHE 6 3\nEVICT 16\n"},
      {"replay-a-15", vesper::Status::ok, "VSP1\nSEGMENT 3 3\nENTRY META aa\nENTRY META bb\nENTRY META cc\nSEALSEG\n"},
      {"replay-a-16", vesper::Status::ok, "VSP1\nDOC 6 f 06\nPAGE 7 0102030405\nSNAPSHOT 3\nRESTORE 3\n"},
      {"replay-a-17", vesper::Status::ok, "VSP1\nPAGE 8 11223344\nCACHE 8 4\nTOUCH 0\nEVICT 4\n"},
      {"replay-a-18", vesper::Status::ok, "VSP1\nDOC 7 g 07\nVIEW 7\nMANIFEST\nSNAPSHOT 4\n"},
      {"replay-a-19", vesper::Status::ok, "VSP1\nSEGMENT 4 1\nENTRY PAGE dd\nSEALSEG\nREPLAY 4\n"},
      {"replay-a-20", vesper::Status::ok, "VSP1\nPAGE 9 f0e0d0c0\nCURSOR 9 1\nSCAN 0\nCOMPACT\n"},
      {"replay-a-21", vesper::Status::ok, "VSP1\nDOC 8 h 08\nPAGE 10 aa55aa55\nCACHE 10 5\n"},
      {"replay-a-22", vesper::Status::ok, "VSP1\nDOC 9 i 09\nVIEW 9\nRENAME 9 west\nMANIFEST\n"},
      {"replay-a-23", vesper::Status::ok, "VSP1\nDOC 10 j 0a\nPAGE 11 bbccdd\nSNAPSHOT 5\nRESTORE 5\nSNAPINFO\n"},
      {"replay-a-24", vesper::Status::ok, "VSP1\nSEGMENT 5 2\nENTRY META 0101\nENTRY META 0202\nSEALSEG\nREPLAY 5\n"},
      {"replay-a-25", vesper::Status::ok, "VSP1\nPAGE 12 01010101\nCACHE 12 2\nTOUCH 0\nTOUCH 0\n"},
      {"replay-a-26", vesper::Status::ok, "VSP1\nKEY 3 aabbccdd\n"},
      {"replay-a-27", vesper::Status::ok, "VSP1\nDOC 11 k 0b\nSNAPSHOT 6\nSNAPINFO\n"},
      {"replay-a-28", vesper::Status::ok, "VSP1\nPAGE 13 22222222\nCURSOR 13 2\nSCAN 0\n"},
      {"replay-a-29", vesper::Status::ok, "VSP1\nDOC 12 l 0c\nPAGE 14 123456\nCACHE 14 3\n"},
      {"replay-a-30", vesper::Status::ok, "VSP1\nSEGMENT 6 1\nENTRY DOC ab\nSEALSEG\n"},
      {"replay-a-31", vesper::Status::invalid_state, "VSP1\nVIEW 99\n"},
      {"replay-a-32", vesper::Status::limit, "VSP1\nPAGE 15 aa\nCURSOR 15 2\n"},
      {"replay-a-33", vesper::Status::invalid_state, "VSP1\nENTRY META aa\n"},
      {"replay-a-34", vesper::Status::invalid_state, "VSP1\nSEALSEG\n"},
      {"replay-a-35", vesper::Status::not_found, "VSP1\nREPLAY 99\n"},
      {"replay-a-36", vesper::Status::not_found, "VSP1\nRESTORE 9\n"},
      {"replay-a-37", vesper::Status::invalid_state, "VSP1\nSNAPINFO\n"},
      {"replay-a-38", vesper::Status::limit, "VSP1\nSEGMENT 0 1\n"},
      {"replay-a-39", vesper::Status::limit, "VSP1\nSEGMENT 7 1\nENTRY bad-kind aa\n"},
      {"replay-a-40", vesper::Status::limit, "VSP1\nKEY 4 00\n"}};

  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
