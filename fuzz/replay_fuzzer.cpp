#include "vesper/engine.hpp"

#include <cstddef>
#include <cstdint>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data,
                                      std::size_t size) {
  vesper::execute_script(data, size, vesper::Profile::all);
  return 0;
}
