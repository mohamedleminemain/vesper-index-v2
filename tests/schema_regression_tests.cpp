#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"deep-b-01", vesper::Status::ok, "VSP1\nDOC 21 a 00\nPAGE 21 aabbcc\nDIGEST page 21\nVERIFY 5\n"},
      {"deep-b-02", vesper::Status::ok, "VSP1\nDOC 22 b 0102\nPAGE 22 bbccdd\nMERGE mixed\nNEXT 2\n"},
      {"deep-b-03", vesper::Status::ok, "VSP1\nDOC 23 c 0203\nISSUE writer 8\nRENEWTICKET 0 3\nCHECKTICKET 0\n"},
      {"deep-b-04", vesper::Status::ok, "VSP1\nDOC 24 d 0304\nPAGE 24 ddee\nNODE 4 36\nSAMPLE all 6\nFOLD\n"},
      {"deep-b-05", vesper::Status::ok, "VSP1\nDOC 25 e 0405\nSNAPSHOT 3\nPAGE 25 eeff\nSNAPSHOT 4\nDIFFSNAP 3 4\n"},
      {"deep-b-06", vesper::Status::ok, "VSP1\nDOC 26 f 0506\nLEDGER archive\nFOLDLEDGER\n"},
      {"deep-b-07", vesper::Status::ok, "VSP1\nDOC 27 g 0607\nPAGE 27 ff00\nQUOTA 8\nENFORCE pages\n"},
      {"deep-b-08", vesper::Status::ok, "VSP1\nDOC 28 h 0708\nLINKS 7 8 9\nRELATE mixed\nWALKREL 0 3\n"},
      {"deep-b-09", vesper::Status::ok, "VSP1\nDOC 29 i 0809\nBLOOM 5 2\nPROBE i\nPLAN docs 4\nEVAL\n"},
      {"deep-b-10", vesper::Status::ok, "VSP1\nPAGE 28 0102030405\nINDEX 1 3\nSEEK 183528\nMETRICS pages\nREDUCE\n"},
      {"deep-b-11", vesper::Status::ok, "VSP1\nDOC 30 j 0910\nNORMALIZE 30\nTOKENS 3\nLABEL 5 north\nRENDER\n"},
      {"deep-b-12", vesper::Status::ok, "VSP1\nPAGE 29 aa55aa55\nPACK 2\nUNPACK\nPALETTE 4\nCOLOR\n"},
      {"deep-b-13", vesper::Status::ok, "VSP1\nROLE guard\nGRANT guard inspect\nCHECK guard inspect\nSIGPLAN 1 1\nFINALIZE\nSIGN\n"},
      {"deep-b-14", vesper::Status::ok, "VSP1\nTIME 2 200\nTIME 3 300\nWINDOW 100 350\nRETAIN 2\n"},
      {"deep-b-15", vesper::Status::ok, "VSP1\nDOC 31 k 1011\nPAGE 30 12345678\nSHARD 3\nREBALANCE 12\nSHARDINFO\n"},
      {"deep-b-16", vesper::Status::ok, "VSP1\nSEGMENT 2 2\nENTRY META aa\nENTRY META bb\nSEALSEG\nREPLAY 2\n"},
      {"deep-b-17", vesper::Status::ok, "VSP1\nPAGE 31 99887766\nCACHE 31 3\nTOUCH 0\nEVICT 4\n"},
      {"deep-b-18", vesper::Status::ok, "VSP1\nDOC 32 l 1112\nSCHEMA 6 a 0 2 b 2 2 c 4 2\nROW 010203040506\nPROJECT\n"},
      {"deep-b-19", vesper::Status::ok, "VSP1\nDOC 33 m 1213\nLINKS 2 5 8\nFOLLOW 0\nEXPORT\n"},
      {"deep-b-20", vesper::Status::ok, "VSP1\nNODE 5 49\nWATCH 5\nAUDIT\n"},
      {"deep-b-21", vesper::Status::limit, "VSP1\nNEXT 0\n"},
      {"deep-b-22", vesper::Status::limit, "VSP1\nCHECKTICKET 8\n"},
      {"deep-b-23", vesper::Status::limit, "VSP1\nQUOTA 0\n"},
      {"deep-b-24", vesper::Status::not_found, "VSP1\nENFORCE missing\n"},
      {"deep-b-25", vesper::Status::not_found, "VSP1\nDIGEST doc 99\n"},
      {"deep-b-26", vesper::Status::unsupported, "VSP1\nLEDGER odd\n"},
      {"deep-b-27", vesper::Status::unsupported, "VSP1\nRELATE odd\n"},
      {"deep-b-28", vesper::Status::limit, "VSP1\nWALKREL 0 0\n"},
      {"deep-b-29", vesper::Status::invalid_state, "VSP1\nCHECKTICKET 0\n"},
      {"deep-b-30", vesper::Status::invalid_state, "VSP1\nMERGE docs\nNEXT 1\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
