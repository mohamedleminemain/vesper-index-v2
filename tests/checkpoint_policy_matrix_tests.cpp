#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"cp-01", vesper::Status::ok, "VSP1\nDOC 1 a 00\nCHECKPOINT 1\nCHECKVIEW 1\n"},
      {"cp-02", vesper::Status::ok, "VSP1\nDOC 2 b 01\nPOLICY documents max 4\nPOLICYCHECK documents max\n"},
      {"cp-03", vesper::Status::ok, "VSP1\nPAGE 1 aa\nPOLICY pages max 4\nPOLICYCHECK pages max\n"},
      {"cp-04", vesper::Status::ok, "VSP1\nDOC 3 c 02\nPAGE 2 bb\nCHECKPOINT 2\nPOLICY documents max 2\nPOLICYCHECK documents max\n"},
      {"cp-05", vesper::Status::limit, "VSP1\nCHECKPOINT 0\n"},
      {"cp-06", vesper::Status::not_found, "VSP1\nCHECKVIEW 9\n"},
      {"cp-07", vesper::Status::limit, "VSP1\nPOLICY docs max 0\n"},
      {"cp-08", vesper::Status::not_found, "VSP1\nPOLICYCHECK docs max\n"},
      {"cp-09", vesper::Status::invalid_state, "VSP1\nCHECKPOINT 1\nCHECKPOINT 1\n"},
      {"cp-10", vesper::Status::invalid_state, "VSP1\nPOLICY documents max 2\nPOLICY documents max 3\n"}};
  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
