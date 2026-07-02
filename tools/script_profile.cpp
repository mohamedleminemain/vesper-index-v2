#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace {

std::vector<std::uint8_t> read_bytes(const char* path) {
  std::ifstream input(path, std::ios::binary);
  return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), {});
}

vesper::Profile parse_profile(const std::string& text) {
  if (text == "archive") return vesper::Profile::archive;
  if (text == "query") return vesper::Profile::query;
  if (text == "codec") return vesper::Profile::codec;
  if (text == "session") return vesper::Profile::session;
  return vesper::Profile::all;
}

}  // namespace

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: vesper-script-profile INPUT PROFILE\n";
    return 2;
  }

  const auto bytes = read_bytes(argv[1]);
  std::uint64_t digest = 0;
  const auto result =
      vesper::execute_script(bytes.data(), bytes.size(),
                             parse_profile(argv[2]), &digest);
  if (!result) {
    std::cerr << result.message << "\n";
    return 1;
  }
  std::cout << "digest=" << digest << "\n";
  std::cout << "profile=" << argv[2] << "\n";
  std::cout << "status=ok\n";
  return 0;
}
