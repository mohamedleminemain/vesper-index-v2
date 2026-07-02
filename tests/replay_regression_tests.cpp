#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"deep-f-01", vesper::Status::ok, "VSP1\nDOC 101 a 00\nPAGE 101 a1a2\nDIGEST events 0\nLEDGER all\nFOLDLEDGER\n"},
      {"deep-f-02", vesper::Status::ok, "VSP1\nDOC 102 b 0102\nISSUE user 3\nCHECKTICKET 0\nQUOTA 5\nENFORCE documents\n"},
      {"deep-f-03", vesper::Status::ok, "VSP1\nDOC 103 c 0203\nNODE 11 169\nSAMPLE all 5\nFOLD\nRELATE mixed\nWALKREL 0 2\n"},
      {"deep-f-04", vesper::Status::ok, "VSP1\nDOC 104 d 0304\nSNAPSHOT 11\nPAGE 102 b2b3\nSNAPSHOT 12\nDIFFSNAP 11 12\n"},
      {"deep-f-05", vesper::Status::ok, "VSP1\nDOC 105 e 0405\nBLOOM 5 2\nPROBE e\nPLAN mixed 5\nEVAL\n"},
      {"deep-f-06", vesper::Status::ok, "VSP1\nPAGE 103 c3c4c5c6\nINDEX 1 2\nSEEK 675943\nMETRICS pages\nREDUCE\n"},
      {"deep-f-07", vesper::Status::ok, "VSP1\nDOC 106 f 0506\nNORMALIZE 106\nTOKENS 2\nPALETTE 1\nCOLOR\n"},
      {"deep-f-08", vesper::Status::ok, "VSP1\nPAGE 104 d4d5d6d7\nPACK 4\nUNPACK\nLABEL 5 north\nRENDER\n"},
      {"deep-f-09", vesper::Status::ok, "VSP1\nROLE ops\nGRANT ops read\nCHECK ops read\nTIME 8 800\nWINDOW 750 850\nRETAIN 1\n"},
      {"deep-f-10", vesper::Status::ok, "VSP1\nDOC 107 g 0607\nPAGE 105 e5e6e7e8\nSHARD 5\nREBALANCE 10\nSHARDINFO\n"},
      {"deep-f-11", vesper::Status::ok, "VSP1\nSEGMENT 6 2\nENTRY META ab\nENTRY META cd\nSEALSEG\nREPLAY 6\n"},
      {"deep-f-12", vesper::Status::ok, "VSP1\nPAGE 106 f6f7f8f9\nCACHE 106 2\nTOUCH 0\nEVICT 2\n"},
      {"deep-f-13", vesper::Status::ok, "VSP1\nDOC 108 h 0708\nSCHEMA 4 a 0 2 b 2 2\nROW 01020304\nPROJECT\n"},
      {"deep-f-14", vesper::Status::ok, "VSP1\nDOC 109 i 0809\nLINKS 3 6 9\nFOLLOW 1\nEXPORT\n"},
      {"deep-f-15", vesper::Status::ok, "VSP1\nNODE 12 196\nWATCH 12\nAUDIT\n"},
      {"deep-f-16", vesper::Status::invalid_state, "VSP1\nCHECKTICKET 0\n"},
      {"deep-f-17", vesper::Status::limit, "VSP1\nISSUE bad! 4\n"},
      {"deep-f-18", vesper::Status::invalid_state, "VSP1\nFOLD\n"},
      {"deep-f-19", vesper::Status::not_found, "VSP1\nDIFFSNAP 99 100\n"},
      {"deep-f-20", vesper::Status::limit, "VSP1\nRELATE nodes\nWALKREL 12 0\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
