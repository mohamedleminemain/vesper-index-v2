#include "vesper/engine.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
namespace { std::vector<std::uint8_t> read_bytes(const char* path) { std::ifstream input(path, std::ios::binary); return std::vector<std::uint8_t>(std::istreambuf_iterator<char>(input), {}); } }
int main(int argc, char** argv) { if (argc != 2) return 2; auto bytes = read_bytes(argv[1]); std::vector<vesper::Command> commands; if (!vesper::parse_script(bytes.data(), bytes.size(), commands)) return 1; vesper::Store store; if (!vesper::execute_commands(store, commands, vesper::Profile::all)) return 1; std::cout << store.relation.edges.size() << "\n"; std::cout << store.relation.walk.size() << "\n"; return 0; }
