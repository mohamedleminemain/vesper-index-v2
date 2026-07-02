#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"replay-b-01", vesper::Status::ok, "VSP1\nDOC 21 north 0102\nPAGE 21 aabb\nSNAPSHOT 21\n"},
      {"replay-b-02", vesper::Status::ok, "VSP1\nDOC 22 south 0203\nPAGE 22 bbcc\nSNAPSHOT 22\nRESTORE 22\n"},
      {"replay-b-03", vesper::Status::ok, "VSP1\nPAGE 23 ccddee\nCACHE 23 3\nTOUCH 0\n"},
      {"replay-b-04", vesper::Status::ok, "VSP1\nPAGE 24 ddeeff00\nCACHE 24 4\nEVICT 12\n"},
      {"replay-b-05", vesper::Status::ok, "VSP1\nSEGMENT 21 1\nENTRY META aa\nSEALSEG\nREPLAY 21\n"},
      {"replay-b-06", vesper::Status::ok, "VSP1\nDOC 23 east 0304\nVIEW 23\nMANIFEST\n"},
      {"replay-b-07", vesper::Status::ok, "VSP1\nDOC 24 west 0405\nRENAME 24 broad\nVIEW 24\n"},
      {"replay-b-08", vesper::Status::ok, "VSP1\nPAGE 25 01020304\nCURSOR 25 1\nSCAN 0\n"},
      {"replay-b-09", vesper::Status::ok, "VSP1\nPAGE 26 1011121314\nCOMPACT\n"},
      {"replay-b-10", vesper::Status::ok, "VSP1\nKEY 21 00112233\nROTATE 21 22 33445566\n"},
      {"replay-b-11", vesper::Status::ok, "VSP1\nSEGMENT 22 2\nENTRY DOC aa\nENTRY META bb\nSEALSEG\n"},
      {"replay-b-12", vesper::Status::ok, "VSP1\nDOC 25 lane 0506\nPAGE 27 21222324\nSNAPSHOT 23\nRESTORE 23\n"},
      {"replay-b-13", vesper::Status::ok, "VSP1\nPAGE 28 ffeeddcc\nCACHE 28 2\nTOUCH 0\nTOUCH 0\n"},
      {"replay-b-14", vesper::Status::ok, "VSP1\nDOC 26 ring 0607\nVIEW 26\nMANIFEST\nSNAPSHOT 24\n"},
      {"replay-b-15", vesper::Status::ok, "VSP1\nSEGMENT 23 3\nENTRY META 0101\nENTRY META 0202\nENTRY META 0303\nSEALSEG\nREPLAY 23\n"},
      {"replay-b-16", vesper::Status::ok, "VSP1\nPAGE 29 abababab\nCURSOR 29 3\nSCAN 0\nCOMPACT\n"},
      {"replay-b-17", vesper::Status::ok, "VSP1\nDOC 27 arc 0708\nPAGE 30 a0a1a2a3\nCACHE 30 4\n"},
      {"replay-b-18", vesper::Status::ok, "VSP1\nDOC 28 weave 0809\nSNAPSHOT 25\nSNAPINFO\n"},
      {"replay-b-19", vesper::Status::ok, "VSP1\nSEGMENT 24 1\nENTRY PAGE abcd\nSEALSEG\n"},
      {"replay-b-20", vesper::Status::ok, "VSP1\nPAGE 31 0a0b0c0d0e\nCACHE 31 5\nEVICT 10\n"},
      {"replay-b-21", vesper::Status::ok, "VSP1\nDOC 29 path 090a\nVIEW 29\nMANIFEST\n"},
      {"replay-b-22", vesper::Status::ok, "VSP1\nDOC 30 fork 0a0b\nPAGE 32 01010101\nSNAPSHOT 26\nRESTORE 26\nSNAPINFO\n"},
      {"replay-b-23", vesper::Status::ok, "VSP1\nKEY 23 aa55aa55\n"},
      {"replay-b-24", vesper::Status::ok, "VSP1\nPAGE 33 99887766\nCURSOR 33 0\nSCAN 0\n"},
      {"replay-b-25", vesper::Status::ok, "VSP1\nSEGMENT 25 2\nENTRY META 1111\nENTRY META 2222\nSEALSEG\n"},
      {"replay-b-26", vesper::Status::ok, "VSP1\nDOC 31 gate 0b0c\nPAGE 34 12341234\nCACHE 34 3\nTOUCH 0\n"},
      {"replay-b-27", vesper::Status::ok, "VSP1\nDOC 32 frame 0c0d\nVIEW 32\nRENAME 32 narrow\nMANIFEST\n"},
      {"replay-b-28", vesper::Status::ok, "VSP1\nPAGE 35 ddddeeee\nCOMPACT\n"},
      {"replay-b-29", vesper::Status::ok, "VSP1\nDOC 33 crest 0d0e\nSNAPSHOT 27\nRESTORE 27\n"},
      {"replay-b-30", vesper::Status::ok, "VSP1\nSEGMENT 26 1\nENTRY DOC 7777\nSEALSEG\nREPLAY 26\n"},
      {"replay-b-31", vesper::Status::limit, "VSP1\nPAGE 36 aa\nCACHE 36 2\n"},
      {"replay-b-32", vesper::Status::limit, "VSP1\nTOUCH 0\n"},
      {"replay-b-33", vesper::Status::invalid_state, "VSP1\nROTATE 90 91 aabb\n"},
      {"replay-b-34", vesper::Status::limit, "VSP1\nSEGMENT 27 0\n"},
      {"replay-b-35", vesper::Status::invalid_state, "VSP1\nSEGMENT 28 1\nSEALSEG\n"},
      {"replay-b-36", vesper::Status::invalid_state, "VSP1\nDOC 40 x 00\nDOC 40 y 11\n"},
      {"replay-b-37", vesper::Status::invalid_state, "VSP1\nPAGE 41 aa\nPAGE 41 bb\n"},
      {"replay-b-38", vesper::Status::invalid_state, "VSP1\nKEY 24 00112233\nKEY 24 44556677\n"},
      {"replay-b-39", vesper::Status::limit, "VSP1\nSEGMENT 29 1\nENTRY bad-kind aa\n"},
      {"replay-b-40", vesper::Status::not_found, "VSP1\nRESTORE 28\n"}};

  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
