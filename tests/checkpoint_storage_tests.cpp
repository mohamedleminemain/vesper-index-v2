#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"storage-01", vesper::Status::ok, "VSP1\nDOC 1 root 00\nSNAPSHOT 1\n"},
      {"storage-02", vesper::Status::ok, "VSP1\nDOC 2 leaf 01\nPAGE 1 aabb\nSNAPSHOT 2\nRESTORE 2\n"},
      {"storage-03", vesper::Status::ok, "VSP1\nSEGMENT 1 1\nENTRY META aa\nSEALSEG\n"},
      {"storage-04", vesper::Status::ok, "VSP1\nSEGMENT 2 2\nENTRY DOC aa\nENTRY PAGE bb\nSEALSEG\nREPLAY 2\n"},
      {"storage-05", vesper::Status::ok, "VSP1\nPAGE 2 01020304\nCACHE 2 2\n"},
      {"storage-06", vesper::Status::ok, "VSP1\nPAGE 3 05060708\nCACHE 3 3\nTOUCH 0\n"},
      {"storage-07", vesper::Status::ok, "VSP1\nPAGE 4 090a0b0c\nCACHE 4 4\nEVICT 8\n"},
      {"storage-08", vesper::Status::ok, "VSP1\nDOC 3 trunk 03\nPAGE 5 ddee\nSNAPSHOT 3\nSNAPINFO\n"},
      {"storage-09", vesper::Status::ok, "VSP1\nDOC 4 bark 04\nPAGE 6 ff00\nSNAPSHOT 4\nRESTORE 4\nSNAPINFO\n"},
      {"storage-10", vesper::Status::ok, "VSP1\nSEGMENT 3 3\nENTRY META 0101\nENTRY META 0202\nENTRY META 0303\nSEALSEG\n"},
      {"storage-11", vesper::Status::ok, "VSP1\nSEGMENT 4 1\nENTRY META 1111\nSEALSEG\nREPLAY 4\n"},
      {"storage-12", vesper::Status::ok, "VSP1\nPAGE 7 12131415\nCACHE 7 2\nTOUCH 0\nTOUCH 0\n"},
      {"storage-13", vesper::Status::ok, "VSP1\nDOC 5 limb 05\nPAGE 8 16171819\nSNAPSHOT 5\nRESTORE 5\n"},
      {"storage-14", vesper::Status::ok, "VSP1\nDOC 6 ring 06\nPAGE 9 20212223\nCACHE 9 3\nEVICT 3\n"},
      {"storage-15", vesper::Status::ok, "VSP1\nSEGMENT 5 2\nENTRY META abcd\nENTRY META dcba\nSEALSEG\nREPLAY 5\n"},
      {"storage-16", vesper::Status::ok, "VSP1\nDOC 7 spark 07\nSNAPSHOT 6\nRESTORE 6\nSNAPINFO\n"},
      {"storage-17", vesper::Status::ok, "VSP1\nPAGE 10 aa55aa55\nCACHE 10 5\n"},
      {"storage-18", vesper::Status::ok, "VSP1\nPAGE 11 33445566\nCACHE 11 2\nEVICT 2\n"},
      {"storage-19", vesper::Status::ok, "VSP1\nSEGMENT 6 1\nENTRY PAGE aa55\nSEALSEG\n"},
      {"storage-20", vesper::Status::ok, "VSP1\nDOC 8 ember 08\nPAGE 12 44556677\nSNAPSHOT 7\n"},
      {"storage-21", vesper::Status::limit, "VSP1\nSNAPSHOT 0\n"},
      {"storage-22", vesper::Status::not_found, "VSP1\nRESTORE 99\n"},
      {"storage-23", vesper::Status::invalid_state, "VSP1\nSNAPINFO\n"},
      {"storage-24", vesper::Status::invalid_state, "VSP1\nSEALSEG\n"},
      {"storage-25", vesper::Status::invalid_state, "VSP1\nENTRY META aa\n"},
      {"storage-26", vesper::Status::not_found, "VSP1\nREPLAY 99\n"},
      {"storage-27", vesper::Status::limit, "VSP1\nPAGE 13 aa\nCACHE 13 2\n"},
      {"storage-28", vesper::Status::limit, "VSP1\nTOUCH 0\n"},
      {"storage-29", vesper::Status::limit, "VSP1\nSEGMENT 0 1\n"},
      {"storage-30", vesper::Status::limit, "VSP1\nSEGMENT 7 1\nENTRY bad-kind aa\n"}};

  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
