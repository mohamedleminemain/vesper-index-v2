#include "vesper/engine.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

int failures = 0;
int tests = 0;

void expect(bool condition, const char* name) {
  ++tests;
  if (!condition) {
    ++failures;
    std::cerr << "FAIL: " << name << "\n";
  }
}

vesper::Result run(const std::string& script, std::uint64_t* digest = nullptr) {
  return vesper::execute_script(
      reinterpret_cast<const std::uint8_t*>(script.data()), script.size(),
      vesper::Profile::all, digest);
}

}  // namespace

int main() {
  std::uint64_t digest = 0;
  expect(run("VSP1\nDOC 7 north 01020304\nRENAME 7 atlas\nVIEW 7\nMANIFEST\n",
             &digest).status == vesper::Status::ok,
         "document manifest workflow");
  expect(digest != 0, "manifest digest produced");
  expect(run("VSP1\nPAGE 9 aabbccdd\nPAGE 10 001122334455\nCACHE 9 2\n"
             "TOUCH 0\nCOMPACT\nCURSOR 9 1\nSCAN 0\n",
             &digest).status == vesper::Status::ok,
         "page cursor workflow");
  expect(run("VSP1\nKEY 1 00112233\n", &digest).status ==
             vesper::Status::ok,
         "key installation");
  expect(run("VSP1\nKEY 3 0011223344556677\nKEYLEASE 8 3 ingest\n"
             "KEYAUDIT active\nKEYRELEASE 8\nKEYAUDIT history\n",
             &digest).status == vesper::Status::ok,
         "key audit workflow");
  expect(run("VSP1\nNODE 4 99\nNODE 5 101\nWATCH 4\nRELATE nodes\n"
             "WALKREL 4 1\nTIME 4 100\nWINDOW 0 200\nWATCHSNAP 4\n"
             "WATCHREVIEW summary\nAUDIT\n",
             &digest).status ==
             vesper::Status::ok,
         "watch audit workflow");
  expect(run("VSP1\nNODE 6 121\nWATCH 6\nTIME 6 90\nWINDOW 0 100\n"
             "WATCHSNAP 6\nWATCHREVIEW timeline\n",
             &digest).status == vesper::Status::ok,
         "watch review workflow");
  expect(run("VSP1\nDOC 11 atlas 01020304\nPAGE 12 aabbccdd\nCHECKPOINT 41\n"
             "INSPECT storage\nCHECKVIEW 41\nPOLICY checkpoints max 65535\n"
             "POLICYCHECK checkpoints max\n",
             &digest).status == vesper::Status::ok,
         "checkpoint policy workflow");
  expect(run("VSP1\nSCHEMA 4 word 0 4\nROW 01020304\nPROJECT\n",
             &digest).status == vesper::Status::ok,
         "schema projection workflow");
  expect(run("VSP1\nLINKS 4 8 15 16\nFOLLOW 3\nEXPORT\n",
             &digest).status == vesper::Status::ok,
         "reference export workflow");
  expect(run("VSP1\nSIGPLAN 2 2\nFINALIZE\nSIGN\n",
             &digest).status == vesper::Status::ok,
         "signature workflow");
  expect(run("VSP1\nVECTOR 8 2 aabb\nDECODE\n",
             &digest).status == vesper::Status::ok,
         "typed vector workflow");
  expect(run("VSP1\nLABEL 5 north\nRENDER\n",
             &digest).status == vesper::Status::ok,
         "label rendering workflow");
  expect(run("VSP1\nPALETTE 15\nCOLOR\n",
             &digest).status == vesper::Status::ok,
         "palette workflow");
  expect(run("VSP1\nINSPECT\n").status == vesper::Status::malformed,
         "reject wrong inspect arity");
  expect(run("bad\n").status == vesper::Status::malformed,
         "reject invalid header");
  expect(run("VSP1\nDOC nope title 00\n").status ==
             vesper::Status::malformed,
         "reject invalid integer");
  expect(run("VSP1\nDOC 1 title xyz\n").status ==
             vesper::Status::malformed,
         "reject invalid hex");
  expect(run("VSP1\nPAGE 1 aa\nCURSOR 1 2\n").status ==
             vesper::Status::limit,
         "reject cursor outside page");
  expect(run("VSP1\nSCHEMA 2 x 0 1\nROW aa\n").status ==
             vesper::Status::malformed,
         "reject row size mismatch");
  expect(run("VSP1\nSIGPLAN 0 1\n").status ==
             vesper::Status::limit,
         "reject empty signature storage");
  expect(run("VSP1\nKEYRELEASE 7\n").status ==
             vesper::Status::not_found,
         "reject missing key lease");
  expect(run("VSP1\nLABEL 6 north\n").status ==
             vesper::Status::malformed,
         "reject label length mismatch");
  expect(run("VSP1\nWATCHREVIEW summary\n").status ==
             vesper::Status::invalid_state,
         "reject empty watch review");
  expect(run("VSP1\nPALETTE 17\n").status ==
             vesper::Status::limit,
         "reject distant palette index");
  expect(run("VSP1\nLINKS 1 2\nFOLLOW 3\n").status ==
             vesper::Status::limit,
         "reject distant reference index");
  expect(run("VSP1\nPOLICYCHECK checkpoints max\n").status ==
             vesper::Status::not_found,
         "reject missing policy");
  expect(run("VSP1\nUNKNOWN 1 2 3\n").status == vesper::Status::ok,
         "ignore extension command");

  std::cout << tests << " checks, " << failures << " failures\n";
  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
