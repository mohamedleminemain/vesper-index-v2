#include "vesper/engine.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: vesper-replay INPUT\n";
    return 2;
  }
  std::ifstream input(argv[1], std::ios::binary);
  if (!input) {
    std::cerr << "cannot open " << argv[1] << "\n";
    return 2;
  }
  std::vector<std::uint8_t> bytes(
      std::istreambuf_iterator<char>(input), {});
  std::uint64_t digest = 0;
  const vesper::Result result =
      vesper::execute_script(bytes.data(), bytes.size(),
                             vesper::Profile::all, &digest);
  if (!result) {
    std::cerr << result.message << "\n";
    return 1;
  }
  std::cout << "ok digest=" << digest << "\n";
  return 0;
}
