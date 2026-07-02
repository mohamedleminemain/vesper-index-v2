#include "test_support.hpp"

int main() {
  vesper_test::Context ctx;
  vesper_test::expect_ok(
      ctx,
      "VSP1\nDOC 1201 east 21222324\nNODE 120 14400\nWATCH 120\nAUDIT\nDIGEST nodes 0\nMERGE docs\nNEXT 1\nISSUE east 3\nCHECKTICKET 0\nSAMPLE nodes 1\nFOLD\nSNAPSHOT 120\nLEDGER query\nFOLDLEDGER\nQUOTA 6\nENFORCE nodes\nRELATE nodes\nWALKREL 120 1\nPLAN graph 5\nEVAL\nMETRICS graph\nREDUCE\nROLE east\nGRANT east inspect\nCHECK east inspect\nTIME 120 12000\nWINDOW 11900 12100\nRETAIN 1\n",
      "story suite k workflow");
  return vesper_test::finish(ctx);
}
