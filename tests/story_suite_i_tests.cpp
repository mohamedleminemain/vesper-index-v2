#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;
  vesper_test::expect_ok(
      ctx,
      "VSP1\nDOC 1001 north 01020304\nPAGE 1001 0011001100110011\nDIGEST doc 1001\nMERGE mixed\nNEXT 2\nISSUE north 5\nCHECKTICKET 0\nSAMPLE all 4\nFOLD\nLEDGER all\nFOLDLEDGER\nQUOTA 8\nENFORCE documents\nRELATE docs\nBLOOM 5 2\nPROBE north\nPLAN mixed 6\nEVAL\nMETRICS all\nREDUCE\nNORMALIZE 1001\nTOKENS 2\nROLE north\nGRANT north read\nCHECK north read\nTIME 100 10000\nWINDOW 9900 10100\nRETAIN 1\n",
      "story suite i workflow");
  return vesper_test::finish(ctx);
}
