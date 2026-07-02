#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"planner-01", vesper::Status::ok, "VSP1\nDOC 1 a 00\nPLAN docs 4\nEVAL\n"},
      {"planner-02", vesper::Status::ok, "VSP1\nPAGE 1 aabbccdd\nPLAN pages 8\nEVAL\n"},
      {"planner-03", vesper::Status::ok, "VSP1\nDOC 2 b 0102\nPAGE 2 11223344\nPLAN mixed 16\nEVAL\n"},
      {"planner-04", vesper::Status::ok, "VSP1\nLINKS 1 2 3\nNODE 1 9\nWATCH 1\nPLAN graph 8\nEVAL\n"},
      {"planner-05", vesper::Status::ok, "VSP1\nDOC 3 c 03\nBLOOM 4 2\nPLAN docs 6\nEVAL\n"},
      {"planner-06", vesper::Status::ok, "VSP1\nPAGE 3 0102030405\nINDEX 2 2\nPLAN pages 5\nEVAL\n"},
      {"planner-07", vesper::Status::ok, "VSP1\nDOC 4 d 04\nPAGE 4 aa55aa55\nBLOOM 6 2\nINDEX 1 2\nPLAN mixed 10\nEVAL\n"},
      {"planner-08", vesper::Status::ok, "VSP1\nDOC 5 e 05\nMETRICS docs\nREDUCE\nPLAN docs 7\nEVAL\n"},
      {"planner-09", vesper::Status::ok, "VSP1\nDOC 6 f 06\nPAGE 5 bbccdd\nMETRICS all\nREDUCE\nPLAN mixed 20\nEVAL\n"},
      {"planner-10", vesper::Status::ok, "VSP1\nPAGE 6 ccddee\nINDEX 3 2\nSEEK 393222\nPLAN pages 9\nEVAL\n"},
      {"planner-11", vesper::Status::ok, "VSP1\nDOC 7 g 07\nBLOOM 8 3\nPROBE g\nPLAN docs 12\nEVAL\n"},
      {"planner-12", vesper::Status::ok, "VSP1\nDOC 8 h 08\nPAGE 7 01010101\nCACHE 7 2\nPLAN mixed 14\nEVAL\n"},
      {"planner-13", vesper::Status::ok, "VSP1\nLINKS 5 8 13\nNODE 2 25\nWATCH 2\nMETRICS graph\nREDUCE\nPLAN graph 15\nEVAL\n"},
      {"planner-14", vesper::Status::ok, "VSP1\nDOC 9 i 09\nPAGE 8 12345678\nBLOOM 5 2\nINDEX 2 3\nPLAN mixed 18\nEVAL\n"},
      {"planner-15", vesper::Status::ok, "VSP1\nDOC 10 j 0a\nPLAN docs 2\nEVAL\nINSPECT query\n"},
      {"planner-16", vesper::Status::ok, "VSP1\nPAGE 9 ff00ff00\nPLAN pages 3\nEVAL\nINSPECT query\n"},
      {"planner-17", vesper::Status::ok, "VSP1\nDOC 11 k 0b\nPAGE 10 00112233\nPLAN mixed 11\nEVAL\nINSPECT query\n"},
      {"planner-18", vesper::Status::ok, "VSP1\nLINKS 2 4 8 16\nPLAN graph 9\nEVAL\n"},
      {"planner-19", vesper::Status::ok, "VSP1\nDOC 12 l 0c\nBLOOM 7 2\nPROBE absent\nPLAN docs 5\nEVAL\n"},
      {"planner-20", vesper::Status::ok, "VSP1\nPAGE 11 abcdef01\nINDEX 4 2\nPLAN pages 4\nEVAL\n"},
      {"planner-21", vesper::Status::limit, "VSP1\nPLAN docs 0\n"},
      {"planner-22", vesper::Status::invalid_state, "VSP1\nPLAN unknown 1\n"},
      {"planner-23", vesper::Status::invalid_state, "VSP1\nEVAL\n"},
      {"planner-24", vesper::Status::limit, "VSP1\nINDEX 0 1\nPLAN pages 1\n"},
      {"planner-25", vesper::Status::limit, "VSP1\nBLOOM 1 1\nPLAN docs 1\n"},
      {"planner-26", vesper::Status::invalid_state, "VSP1\nREDUCE\nPLAN docs 1\n"},
      {"planner-27", vesper::Status::invalid_state, "VSP1\nPROBE x\nPLAN docs 1\n"},
      {"planner-28", vesper::Status::invalid_state, "VSP1\nSEEK 1\nPLAN pages 1\n"},
      {"planner-29", vesper::Status::unsupported, "VSP1\nINSPECT weird\nPLAN docs 1\n"},
      {"planner-30", vesper::Status::ok, "VSP1\nDOC 13 m 0d\nPAGE 12 0f0e0d0c\nMETRICS all\nREDUCE\nPLAN mixed 13\nEVAL\n"}};

  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
