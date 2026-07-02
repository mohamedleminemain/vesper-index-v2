#pragma once

#include "vesper/engine.hpp"

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace vesper_test {

struct ScriptCase {
  const char* name;
  vesper::Status status;
  const char* script;
};

struct Context {
  int failures = 0;
  int checks = 0;

  void expect(bool condition, const char* label) {
    ++checks;
    if (!condition) {
      ++failures;
      std::cerr << "FAIL: " << label << "\n";
    }
  }
};

inline vesper::Result run_script(const std::string& script,
                                 vesper::Profile profile = vesper::Profile::all,
                                 std::uint64_t* digest = nullptr) {
  return vesper::execute_script(
      reinterpret_cast<const std::uint8_t*>(script.data()), script.size(),
      profile, digest);
}

inline void expect_ok(Context& ctx, const std::string& script,
                      const char* label,
                      vesper::Profile profile = vesper::Profile::all) {
  const auto result = run_script(script, profile);
  ctx.expect(result.status == vesper::Status::ok, label);
}

inline void expect_status(Context& ctx, const std::string& script,
                          vesper::Status status, const char* label,
                          vesper::Profile profile = vesper::Profile::all) {
  const auto result = run_script(script, profile);
  ctx.expect(result.status == status, label);
}

inline void expect_digest(Context& ctx, const std::string& script,
                          const char* label,
                          vesper::Profile profile = vesper::Profile::all) {
  std::uint64_t digest = 0;
  const auto result = run_script(script, profile, &digest);
  ctx.expect(result.status == vesper::Status::ok && digest != 0, label);
}

inline int finish(Context& ctx) {
  std::cout << ctx.checks << " checks, " << ctx.failures << " failures\n";
  return ctx.failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

inline void run_cases(Context& ctx, const std::vector<ScriptCase>& cases,
                      vesper::Profile profile = vesper::Profile::all) {
  for (const auto& test : cases) {
    const auto result = run_script(test.script, profile);
    ctx.expect(result.status == test.status, test.name);
  }
}

}  // namespace vesper_test
