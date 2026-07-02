#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"deep-c-01", vesper::Status::ok, "VSP1\nDOC 41 a 00\nPAGE 41 0102\nDIGEST doc 41\nDIGEST page 41\nVERIFY 1\n"},
      {"deep-c-02", vesper::Status::ok, "VSP1\nDOC 42 b 0102\nPAGE 42 1122\nMERGE docs\nNEXT 1\n"},
      {"deep-c-03", vesper::Status::ok, "VSP1\nDOC 43 c 0203\nISSUE alpha 5\nCHECKTICKET 0\n"},
      {"deep-c-04", vesper::Status::ok, "VSP1\nDOC 44 d 0304\nPAGE 43 aabbcc\nSAMPLE docs 1\nFOLD\n"},
      {"deep-c-05", vesper::Status::ok, "VSP1\nDOC 45 e 0405\nSNAPSHOT 5\nDOC 46 f 0506\nSNAPSHOT 6\nDIFFSNAP 5 6\n"},
      {"deep-c-06", vesper::Status::ok, "VSP1\nDOC 47 g 0607\nLEDGER query\nFOLDLEDGER\n"},
      {"deep-c-07", vesper::Status::ok, "VSP1\nDOC 48 h 0708\nQUOTA 5\nENFORCE documents\n"},
      {"deep-c-08", vesper::Status::ok, "VSP1\nLINKS 10 20 30\nRELATE links\nWALKREL 0 2\n"},
      {"deep-c-09", vesper::Status::ok, "VSP1\nDOC 49 i 0809\nPAGE 44 bbccdd\nBLOOM 6 2\nPROBE i\n"},
      {"deep-c-10", vesper::Status::ok, "VSP1\nPAGE 45 1122334455\nINDEX 2 2\nSEEK 294957\n"},
      {"deep-c-11", vesper::Status::ok, "VSP1\nDOC 50 j 0910\nPLAN docs 7\nEVAL\n"},
      {"deep-c-12", vesper::Status::ok, "VSP1\nDOC 51 k 1011\nMETRICS docs\nREDUCE\n"},
      {"deep-c-13", vesper::Status::ok, "VSP1\nDOC 52 l 1112\nNORMALIZE 52\nTOKENS 1\n"},
      {"deep-c-14", vesper::Status::ok, "VSP1\nPAGE 46 22222222\nPACK 4\nUNPACK\n"},
      {"deep-c-15", vesper::Status::ok, "VSP1\nROLE reader\nGRANT reader read\nCHECK reader write\n"},
      {"deep-c-16", vesper::Status::ok, "VSP1\nTIME 4 400\nWINDOW 350 450\nRETAIN 1\n"},
      {"deep-c-17", vesper::Status::ok, "VSP1\nDOC 53 m 1213\nPAGE 47 ff00ff00\nSHARD 4\nSHARDINFO\n"},
      {"deep-c-18", vesper::Status::ok, "VSP1\nSEGMENT 3 1\nENTRY META abcd\nSEALSEG\n"},
      {"deep-c-19", vesper::Status::ok, "VSP1\nPAGE 48 33334444\nCACHE 48 2\nTOUCH 0\n"},
      {"deep-c-20", vesper::Status::ok, "VSP1\nDOC 54 n 1314\nLINKS 1 2\nFOLLOW 1\nEXPORT\n"},
      {"deep-c-21", vesper::Status::ok, "VSP1\nNODE 6 64\nWATCH 6\nAUDIT\n"},
      {"deep-c-22", vesper::Status::invalid_state, "VSP1\nFOLDLEDGER\n"},
      {"deep-c-23", vesper::Status::not_found, "VSP1\nENFORCE pages\n"},
      {"deep-c-24", vesper::Status::not_found, "VSP1\nWALKREL 1 2\n"},
      {"deep-c-25", vesper::Status::invalid_state, "VSP1\nLEDGER all\nFOLDLEDGER\nWALKREL 0 1\n"},
      {"deep-c-26", vesper::Status::limit, "VSP1\nQUOTA 5000\n"},
      {"deep-c-27", vesper::Status::limit, "VSP1\nISSUE reader 0\n"},
      {"deep-c-28", vesper::Status::limit, "VSP1\nRENEWTICKET 0 0\n"},
      {"deep-c-29", vesper::Status::unsupported, "VSP1\nSAMPLE odd 2\n"},
      {"deep-c-30", vesper::Status::limit, "VSP1\nRELATE docs\nWALKREL 1 0\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
