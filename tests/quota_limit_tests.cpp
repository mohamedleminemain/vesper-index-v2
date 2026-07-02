#include "test_support.hpp"

#include <vector>

namespace {

struct LimitCase {
  const char* name;
  const char* script;
  vesper::Status status;
};

}  // namespace

int main() {
  vesper_test::Context ctx;
  const std::vector<LimitCase> cases{
      {"limit-01", "VSP1\nSEGMENT 1 0\n", vesper::Status::limit},
      {"limit-02", "VSP1\nSEGMENT 0 1\n", vesper::Status::limit},
      {"limit-03", "VSP1\nCACHE 1 1\n", vesper::Status::not_found},
      {"limit-04", "VSP1\nROLE admin!\n", vesper::Status::limit},
      {"limit-05", "VSP1\nTIME 0 1\n", vesper::Status::limit},
      {"limit-06", "VSP1\nRETAIN 0\n", vesper::Status::limit},
      {"limit-07", "VSP1\nPACK 256\n", vesper::Status::limit},
      {"limit-08", "VSP1\nTOKENS 0\n", vesper::Status::invalid_state},
      {"limit-09", "VSP1\nPLAN docs 0\n", vesper::Status::limit},
      {"limit-10", "VSP1\nINDEX 0 1\n", vesper::Status::limit},
      {"limit-11", "VSP1\nBLOOM 2 0\n", vesper::Status::limit},
      {"limit-12", "VSP1\nPALETTE 17\n", vesper::Status::limit},
      {"limit-13", "VSP1\nSIGPLAN 0 1\n", vesper::Status::limit},
      {"limit-14", "VSP1\nDOC 1 title zz\n", vesper::Status::malformed},
      {"limit-15", "VSP1\nPAGE 1 aa\nCURSOR 1 2\n", vesper::Status::limit},
      {"limit-16", "VSP1\nFOLLOW 99\n", vesper::Status::limit},
      {"limit-17", "VSP1\nSEEK 1\n", vesper::Status::invalid_state},
      {"limit-18", "VSP1\nREDUCE\n", vesper::Status::invalid_state},
      {"limit-19", "VSP1\nPROBE hello\n", vesper::Status::invalid_state},
      {"limit-20", "VSP1\nTOUCH 1\n", vesper::Status::limit},
      {"limit-21",
       "VSP1\nDOC 1 a 00\nNORMALIZE 1\nTOKENS 0\n",
       vesper::Status::limit},
      {"limit-22",
       "VSP1\nDOC 1 a 00\nBLOOM 1 1\n",
       vesper::Status::limit},
      {"limit-23",
       "VSP1\nPAGE 1 aa\nPACK 0\n",
       vesper::Status::limit},
      {"limit-24",
       "VSP1\nPAGE 1 aa\nCACHE 1 2\n",
       vesper::Status::limit},
      {"limit-25",
       "VSP1\nROLE worker\nGRANT worker BadPerm\n",
       vesper::Status::limit},
      {"limit-26",
       "VSP1\nTIME 1 0\n",
       vesper::Status::limit},
      {"limit-27",
       "VSP1\nSHARD 0\n",
       vesper::Status::limit},
      {"limit-28",
       "VSP1\nREBALANCE 0\n",
       vesper::Status::invalid_state},
      {"limit-29",
       "VSP1\nSNAPSHOT 0\n",
       vesper::Status::limit},
      {"limit-30",
       "VSP1\nSEGMENT 2 1\nENTRY bad-kind aa\n",
       vesper::Status::limit},
      {"limit-31",
       "VSP1\nDOC 1 a 00\nPLAN unknown 1\n",
       vesper::Status::invalid_state},
      {"limit-32",
       "VSP1\nWINDOW 2 1\n",
       vesper::Status::malformed},
      {"limit-33",
       "VSP1\nLABEL 6 north\n",
       vesper::Status::malformed},
      {"limit-34",
       "VSP1\nPAGE 1 aa\nCACHE 1 0\n",
       vesper::Status::limit},
      {"limit-35",
       "VSP1\nDOC 1 a 00\nMETRICS weird\n",
       vesper::Status::unsupported},
      {"limit-36",
       "VSP1\nINSPECT weird\n",
       vesper::Status::unsupported},
      {"limit-37",
       "VSP1\nDOC 1 a 00\nROLE admin\nGRANT admin read\nGRANT admin read\n",
       vesper::Status::invalid_state},
      {"limit-38",
       "VSP1\nDOC 1 a 00\nSEGMENT 1 1\nENTRY META aa\nENTRY META bb\n",
       vesper::Status::limit},
      {"limit-39",
       "VSP1\nDOC 1 a 00\nSNAPINFO\n",
       vesper::Status::not_found},
      {"limit-40",
       "VSP1\nDOC 1 a 00\nUNPACK\n",
       vesper::Status::invalid_state}};

  for (const auto& test : cases) {
    const auto result = vesper_test::run_script(test.script);
    ctx.expect(result.status == test.status, test.name);
  }

  return vesper_test::finish(ctx);
}
