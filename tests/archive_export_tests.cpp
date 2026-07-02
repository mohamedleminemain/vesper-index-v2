#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;
  vesper_test::expect_ok(
      ctx,
      "VSP1\nDOC 1101 south 11121314\nPAGE 1101 aa55aa55aa55aa55\nSEGMENT 110 1\nENTRY META abcd\nSEALSEG\nREPLAY 110\nSNAPSHOT 110\nDIGEST events 0\nMERGE pages\nNEXT 1\nISSUE south 4\nRENEWTICKET 0 4\nCHECKTICKET 0\nSAMPLE pages 1\nFOLD\nLEDGER archive\nFOLDLEDGER\nQUOTA 7\nENFORCE events\nRELATE mixed\nWALKREL 0 1\nINDEX 2 2\nSEEK 721437\nPACK 8\nUNPACK\nINSPECT storage\n",
      "story suite j workflow");
  return vesper_test::finish(ctx);
}
