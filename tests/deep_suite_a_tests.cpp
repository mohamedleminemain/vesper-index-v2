#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"deep-a-01", vesper::Status::ok, "VSP1\nDOC 1 aa 00\nPAGE 1 aabb\nDIGEST doc 1\nMERGE mixed\nNEXT 1\n"},
      {"deep-a-02", vesper::Status::ok, "VSP1\nDOC 2 bb 0102\nPAGE 2 bbcc\nISSUE reader 4\nCHECKTICKET 0\n"},
      {"deep-a-03", vesper::Status::ok, "VSP1\nDOC 3 cc 0203\nNODE 1 9\nSAMPLE all 4\nFOLD\n"},
      {"deep-a-04", vesper::Status::ok, "VSP1\nDOC 4 dd 0304\nSNAPSHOT 1\nDOC 5 ee 0405\nSNAPSHOT 2\nDIFFSNAP 1 2\n"},
      {"deep-a-05", vesper::Status::ok, "VSP1\nDOC 6 ff 0506\nLEDGER all\nFOLDLEDGER\nQUOTA 4\nENFORCE documents\n"},
      {"deep-a-06", vesper::Status::ok, "VSP1\nLINKS 2 4 6\nRELATE links\nWALKREL 0 2\n"},
      {"deep-a-07", vesper::Status::ok, "VSP1\nDOC 7 gg 0607\nPAGE 3 ccddee\nBLOOM 4 2\nPROBE gg\n"},
      {"deep-a-08", vesper::Status::ok, "VSP1\nPAGE 4 ddeeff00\nINDEX 2 2\nSEEK 262148\n"},
      {"deep-a-09", vesper::Status::ok, "VSP1\nDOC 8 hh 0708\nPLAN docs 3\nEVAL\n"},
      {"deep-a-10", vesper::Status::ok, "VSP1\nDOC 9 ii 0809\nMETRICS docs\nREDUCE\n"},
      {"deep-a-11", vesper::Status::ok, "VSP1\nDOC 10 jj 0910\nNORMALIZE 10\nTOKENS 2\n"},
      {"deep-a-12", vesper::Status::ok, "VSP1\nPAGE 5 01010101\nPACK 4\nUNPACK\n"},
      {"deep-a-13", vesper::Status::ok, "VSP1\nROLE analyst\nGRANT analyst read\nCHECK analyst read\n"},
      {"deep-a-14", vesper::Status::ok, "VSP1\nTIME 1 100\nWINDOW 50 150\nRETAIN 1\n"},
      {"deep-a-15", vesper::Status::ok, "VSP1\nDOC 11 kk 1011\nPAGE 6 22334455\nSHARD 4\nSHARDINFO\n"},
      {"deep-a-16", vesper::Status::ok, "VSP1\nDOC 12 ll 1112\nSEGMENT 1 1\nENTRY META aa\nSEALSEG\nREPLAY 1\n"},
      {"deep-a-17", vesper::Status::ok, "VSP1\nDOC 13 mm 1213\nPAGE 7 33445566\nCACHE 7 2\nTOUCH 0\n"},
      {"deep-a-18", vesper::Status::ok, "VSP1\nDOC 14 nn 1314\nSCHEMA 4 a 0 2 b 2 2\nROW 01020304\nPROJECT\n"},
      {"deep-a-19", vesper::Status::ok, "VSP1\nDOC 15 oo 1415\nLINKS 1 3 5\nFOLLOW 1\nEXPORT\n"},
      {"deep-a-20", vesper::Status::ok, "VSP1\nNODE 2 16\nWATCH 2\nAUDIT\n"},
      {"deep-a-21", vesper::Status::invalid_state, "VSP1\nVERIFY 1\n"},
      {"deep-a-22", vesper::Status::unsupported, "VSP1\nMERGE odd\n"},
      {"deep-a-23", vesper::Status::limit, "VSP1\nISSUE BAD 4\n"},
      {"deep-a-24", vesper::Status::invalid_state, "VSP1\nFOLD\n"},
      {"deep-a-25", vesper::Status::not_found, "VSP1\nDIFFSNAP 1 2\n"},
      {"deep-a-26", vesper::Status::invalid_state, "VSP1\nFOLDLEDGER\n"},
      {"deep-a-27", vesper::Status::invalid_state, "VSP1\nENFORCE documents\n"},
      {"deep-a-28", vesper::Status::invalid_state, "VSP1\nRELATE mixed\n"},
      {"deep-a-29", vesper::Status::limit, "VSP1\nSAMPLE all 0\n"},
      {"deep-a-30", vesper::Status::not_found, "VSP1\nWALKREL 9 2\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
