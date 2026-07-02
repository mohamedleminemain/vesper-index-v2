#include "test_support.hpp"

#include <vector>

int main() {
  vesper_test::Context ctx;
  const std::vector<vesper_test::ScriptCase> cases{
      {"security-01", vesper::Status::ok, "VSP1\nROLE analyst\nGRANT analyst read\nCHECK analyst read\n"},
      {"security-02", vesper::Status::ok, "VSP1\nROLE writer\nGRANT writer read\nGRANT writer write\nCHECK writer write\n"},
      {"security-03", vesper::Status::ok, "VSP1\nTIME 1 100\nWINDOW 50 150\nRETAIN 1\n"},
      {"security-04", vesper::Status::ok, "VSP1\nTIME 2 200\nTIME 3 300\nWINDOW 150 350\nRETAIN 2\n"},
      {"security-05", vesper::Status::ok, "VSP1\nSIGPLAN 1 1\nFINALIZE\nSIGN\n"},
      {"security-06", vesper::Status::ok, "VSP1\nROLE reader\nGRANT reader read\nCHECK reader write\n"},
      {"security-07", vesper::Status::ok, "VSP1\nROLE audit\nGRANT audit inspect\nCHECK audit inspect\n"},
      {"security-08", vesper::Status::ok, "VSP1\nTIME 4 400\nRETAIN 1\n"},
      {"security-09", vesper::Status::ok, "VSP1\nSIGPLAN 2 2\nFINALIZE\nSIGN\nROLE signer\nGRANT signer approve\nCHECK signer approve\n"},
      {"security-10", vesper::Status::ok, "VSP1\nROLE ops\nGRANT ops read\nGRANT ops review\nCHECK ops review\n"},
      {"security-11", vesper::Status::ok, "VSP1\nTIME 5 500\nWINDOW 450 550\n"},
      {"security-12", vesper::Status::ok, "VSP1\nROLE qa\nGRANT qa inspect\nCHECK qa write\n"},
      {"security-13", vesper::Status::ok, "VSP1\nSIGPLAN 3 3\nFINALIZE\nSIGN\n"},
      {"security-14", vesper::Status::ok, "VSP1\nTIME 6 600\nTIME 7 700\nTIME 8 800\nWINDOW 650 850\nRETAIN 2\n"},
      {"security-15", vesper::Status::ok, "VSP1\nROLE alpha\nGRANT alpha read\nROLE beta\nGRANT beta write\nCHECK beta read\n"},
      {"security-16", vesper::Status::limit, "VSP1\nROLE BAD\n"},
      {"security-17", vesper::Status::not_found, "VSP1\nGRANT nobody read\n"},
      {"security-18", vesper::Status::not_found, "VSP1\nCHECK nobody read\n"},
      {"security-19", vesper::Status::limit, "VSP1\nTIME 0 10\n"},
      {"security-20", vesper::Status::malformed, "VSP1\nWINDOW 20 10\n"},
      {"security-21", vesper::Status::limit, "VSP1\nRETAIN 0\n"},
      {"security-22", vesper::Status::invalid_state, "VSP1\nSIGN\n"},
      {"security-23", vesper::Status::invalid_state, "VSP1\nFINALIZE\n"},
      {"security-24", vesper::Status::invalid_state, "VSP1\nROLE same\nGRANT same read\nGRANT same read\n"},
      {"security-25", vesper::Status::limit, "VSP1\nROLE one\nGRANT one BadPerm\n"},
      {"security-26", vesper::Status::ok, "VSP1\nROLE gamma\nGRANT gamma export\nCHECK gamma export\nTIME 9 900\nWINDOW 800 950\n"},
      {"security-27", vesper::Status::ok, "VSP1\nSIGPLAN 4 4\nFINALIZE\nSIGN\nROLE verifier\nGRANT verifier check\nCHECK verifier check\n"},
      {"security-28", vesper::Status::ok, "VSP1\nTIME 10 1000\nWINDOW 900 1100\nRETAIN 1\n"},
      {"security-29", vesper::Status::ok, "VSP1\nROLE delta\nGRANT delta read\nCHECK delta missing\n"},
      {"security-30", vesper::Status::ok, "VSP1\nROLE epsilon\nGRANT epsilon archive\nCHECK epsilon archive\n"}};

  vesper_test::run_cases(ctx, cases);
  return vesper_test::finish(ctx);
}
