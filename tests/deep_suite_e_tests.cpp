#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"deep-e-01", vesper::Status::ok, "VSP1\nDOC 81 a 00\nPAGE 81 aa55\nDIGEST doc 81\nDIGEST page 81\n"},
      {"deep-e-02", vesper::Status::ok, "VSP1\nDOC 82 b 0102\nMERGE docs\nNEXT 1\nLEDGER archive\nFOLDLEDGER\n"},
      {"deep-e-03", vesper::Status::ok, "VSP1\nDOC 83 c 0203\nISSUE alpha 9\nCHECKTICKET 0\nQUOTA 4\nENFORCE documents\n"},
      {"deep-e-04", vesper::Status::ok, "VSP1\nDOC 84 d 0304\nNODE 9 121\nSAMPLE nodes 1\nFOLD\nRELATE nodes\nWALKREL 9 1\n"},
      {"deep-e-05", vesper::Status::ok, "VSP1\nDOC 85 e 0405\nSNAPSHOT 9\nDOC 86 f 0506\nSNAPSHOT 10\nDIFFSNAP 9 10\n"},
      {"deep-e-06", vesper::Status::ok, "VSP1\nDOC 87 g 0607\nPAGE 82 bb66\nBLOOM 4 2\nPROBE g\nPLAN docs 2\nEVAL\n"},
      {"deep-e-07", vesper::Status::ok, "VSP1\nPAGE 83 cc77dd88\nINDEX 2 2\nSEEK 543571\nMETRICS pages\nREDUCE\n"},
      {"deep-e-08", vesper::Status::ok, "VSP1\nDOC 88 h 0708\nNORMALIZE 88\nTOKENS 2\nPACK 4\nUNPACK\n"},
      {"deep-e-09", vesper::Status::ok, "VSP1\nROLE audit\nGRANT audit inspect\nCHECK audit inspect\nTIME 7 700\nWINDOW 650 750\nRETAIN 1\n"},
      {"deep-e-10", vesper::Status::ok, "VSP1\nDOC 89 i 0809\nPAGE 84 dd88ee99\nSHARD 4\nSHARDINFO\nSEGMENT 5 1\nENTRY META aa\nSEALSEG\n"},
      {"deep-e-11", vesper::Status::ok, "VSP1\nPAGE 85 ee99ff00\nCACHE 85 2\nTOUCH 0\nSCHEMA 4 a 0 2 b 2 2\nROW 01020304\nPROJECT\n"},
      {"deep-e-12", vesper::Status::ok, "VSP1\nDOC 90 j 0910\nLINKS 4 6 8\nFOLLOW 2\nEXPORT\nNODE 10 144\nWATCH 10\nAUDIT\n"},
      {"deep-e-13", vesper::Status::ok, "VSP1\nDOC 91 k 1011\nLEDGER query\nFOLDLEDGER\nRELATE docs\nWALKREL 91 1\n"},
      {"deep-e-14", vesper::Status::ok, "VSP1\nDOC 92 l 1112\nQUOTA 6\nENFORCE events\nISSUE beta 4\nRENEWTICKET 0 4\nCHECKTICKET 0\n"},
      {"deep-e-15", vesper::Status::ok, "VSP1\nDOC 93 m 1213\nSAMPLE docs 1\nFOLD\nDIGEST events 0\nVERIFY 7\n"},
      {"deep-e-16", vesper::Status::limit, "VSP1\nRELATE docs\nWALKREL 1 0\n"},
      {"deep-e-17", vesper::Status::limit, "VSP1\nQUOTA 4097\n"},
      {"deep-e-18", vesper::Status::not_found, "VSP1\nENFORCE links\n"},
      {"deep-e-19", vesper::Status::invalid_state, "VSP1\nLEDGER archive\nRELATE docs\nWALKREL 1 1\n"},
      {"deep-e-20", vesper::Status::unsupported, "VSP1\nLEDGER mystery\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
