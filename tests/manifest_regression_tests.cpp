#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"deep-d-01", vesper::Status::ok, "VSP1\nDOC 61 a 00\nPAGE 61 aabb\nDIGEST events 0\nVERIFY 2\n"},
      {"deep-d-02", vesper::Status::ok, "VSP1\nDOC 62 b 0102\nPAGE 62 bbcc\nMERGE pages\nNEXT 1\n"},
      {"deep-d-03", vesper::Status::ok, "VSP1\nDOC 63 c 0203\nISSUE delta 7\nRENEWTICKET 0 2\nCHECKTICKET 0\n"},
      {"deep-d-04", vesper::Status::ok, "VSP1\nDOC 64 d 0304\nPAGE 63 ccddee\nNODE 7 81\nSAMPLE nodes 1\nFOLD\n"},
      {"deep-d-05", vesper::Status::ok, "VSP1\nDOC 65 e 0405\nSNAPSHOT 7\nPAGE 64 ddeeff\nSNAPSHOT 8\nDIFFSNAP 7 8\n"},
      {"deep-d-06", vesper::Status::ok, "VSP1\nDOC 66 f 0506\nLEDGER codec\nFOLDLEDGER\n"},
      {"deep-d-07", vesper::Status::ok, "VSP1\nDOC 67 g 0607\nQUOTA 9\nENFORCE events\n"},
      {"deep-d-08", vesper::Status::ok, "VSP1\nDOC 68 h 0708\nLINKS 12 24 36\nRELATE mixed\nWALKREL 0 3\n"},
      {"deep-d-09", vesper::Status::ok, "VSP1\nDOC 69 i 0809\nBLOOM 7 3\nPROBE i\nPLAN mixed 9\nEVAL\n"},
      {"deep-d-10", vesper::Status::ok, "VSP1\nPAGE 65 010203040506\nINDEX 3 2\nSEEK 425025\nMETRICS pages\nREDUCE\n"},
      {"deep-d-11", vesper::Status::ok, "VSP1\nDOC 70 j 0910\nNORMALIZE 70\nTOKENS 4\nPALETTE 2\nCOLOR\n"},
      {"deep-d-12", vesper::Status::ok, "VSP1\nPAGE 66 44445555\nPACK 2\nUNPACK\nLABEL 5 north\nRENDER\n"},
      {"deep-d-13", vesper::Status::ok, "VSP1\nROLE ops\nGRANT ops read\nCHECK ops read\nSIGPLAN 2 2\nFINALIZE\nSIGN\n"},
      {"deep-d-14", vesper::Status::ok, "VSP1\nTIME 5 500\nTIME 6 600\nWINDOW 450 650\nRETAIN 2\n"},
      {"deep-d-15", vesper::Status::ok, "VSP1\nDOC 71 k 1011\nPAGE 67 55556666\nSHARD 3\nREBALANCE 8\nSHARDINFO\n"},
      {"deep-d-16", vesper::Status::ok, "VSP1\nSEGMENT 4 2\nENTRY META 1111\nENTRY META 2222\nSEALSEG\nREPLAY 4\n"},
      {"deep-d-17", vesper::Status::ok, "VSP1\nPAGE 68 66667777\nCACHE 68 2\nTOUCH 0\nEVICT 2\n"},
      {"deep-d-18", vesper::Status::ok, "VSP1\nDOC 72 l 1112\nSCHEMA 4 a 0 2 b 2 2\nROW 01020304\nPROJECT\n"},
      {"deep-d-19", vesper::Status::ok, "VSP1\nDOC 73 m 1213\nLINKS 9 8 7\nFOLLOW 2\nEXPORT\n"},
      {"deep-d-20", vesper::Status::ok, "VSP1\nNODE 8 100\nWATCH 8\nAUDIT\n"},
      {"deep-d-21", vesper::Status::limit, "VSP1\nLEDGER \n"},
      {"deep-d-22", vesper::Status::not_found, "VSP1\nDIGEST page 99\n"},
      {"deep-d-23", vesper::Status::invalid_state, "VSP1\nRELATE links\nWALKREL 9 2\n"},
      {"deep-d-24", vesper::Status::invalid_state, "VSP1\nLEDGER all\nENFORCE documents\n"},
      {"deep-d-25", vesper::Status::unsupported, "VSP1\nRELATE odd\n"},
      {"deep-d-26", vesper::Status::limit, "VSP1\nQUOTA 0\n"},
      {"deep-d-27", vesper::Status::limit, "VSP1\nISSUE ticket 5000\n"},
      {"deep-d-28", vesper::Status::invalid_state, "VSP1\nCHECKTICKET 0\n"},
      {"deep-d-29", vesper::Status::limit, "VSP1\nSAMPLE docs 999\n"},
      {"deep-d-30", vesper::Status::invalid_state, "VSP1\nDIFFSNAP 1 1\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
