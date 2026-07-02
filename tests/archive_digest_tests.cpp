#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;
  vesper_test::expect_ok(
      ctx,
      "VSP1\nDOC 1301 west 31323334\nPAGE 1301 1234567890abcdef\nCACHE 1301 4\nTOUCH 0\nDIGEST page 1301\nMERGE pages\nNEXT 1\nISSUE west 8\nCHECKTICKET 0\nSAMPLE pages 1\nFOLD\nLEDGER codec\nFOLDLEDGER\nQUOTA 9\nENFORCE pages\nBLOOM 6 2\nPROBE west\nINDEX 2 2\nSEEK 852373\nPLAN pages 4\nEVAL\nMETRICS pages\nREDUCE\nNORMALIZE 1301\nTOKENS 2\nPACK 8\nUNPACK\nROLE west\nGRANT west read\nCHECK west read\nSHARD 3\nSHARDINFO\n",
      "story suite l workflow");
  return vesper_test::finish(ctx);
}
