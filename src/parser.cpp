#include "vesper/engine.hpp"

#include <algorithm>
#include <cctype>
#include <string>

namespace vesper {
namespace {

bool valid_token_char(unsigned char value) {
  return value >= 0x21 && value <= 0x7e;
}

std::vector<std::string> tokenize(std::string_view line) {
  std::vector<std::string> tokens;
  std::size_t cursor = 0;
  while (cursor < line.size()) {
    while (cursor < line.size() &&
           std::isspace(static_cast<unsigned char>(line[cursor]))) {
      ++cursor;
    }
    if (cursor == line.size() || line[cursor] == '#') break;
    std::string token;
    if (line[cursor] == '"') {
      ++cursor;
      while (cursor < line.size() && line[cursor] != '"') {
        if (line[cursor] == '\\' && cursor + 1 < line.size()) {
          ++cursor;
          switch (line[cursor]) {
            case 'n':
              token.push_back('\n');
              break;
            case 't':
              token.push_back('\t');
              break;
            default:
              token.push_back(line[cursor]);
              break;
          }
        } else {
          token.push_back(line[cursor]);
        }
        ++cursor;
      }
      if (cursor < line.size()) ++cursor;
    } else {
      const std::size_t start = cursor;
      while (cursor < line.size() &&
             !std::isspace(static_cast<unsigned char>(line[cursor]))) {
        ++cursor;
      }
      token.assign(line.substr(start, cursor - start));
    }
    tokens.push_back(std::move(token));
  }
  return tokens;
}

}  // namespace

Result parse_script(const std::uint8_t* data, std::size_t size,
                    std::vector<Command>& commands) {
  commands.clear();
  if (!data || size < 5 || size > (1U << 20U)) {
    return Result::fail(Status::malformed, "script size invalid");
  }
  const std::string_view input(reinterpret_cast<const char*>(data), size);
  if (input.substr(0, 5) != "VSP1\n") {
    return Result::fail(Status::malformed, "missing VSP1 header");
  }
  std::size_t begin = 5;
  std::size_t line_number = 2;
  while (begin <= input.size()) {
    std::size_t end = input.find('\n', begin);
    if (end == std::string_view::npos) end = input.size();
    std::string_view line = input.substr(begin, end - begin);
    if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
    if (line.size() > 8192 ||
        !std::all_of(line.begin(), line.end(), [](char ch) {
          const auto value = static_cast<unsigned char>(ch);
          return value == '\t' || valid_token_char(value) || value == ' ';
        })) {
      return Result::fail(Status::malformed, "invalid script line");
    }
    auto tokens = tokenize(line);
    if (!tokens.empty()) {
      Command command;
      command.verb = std::move(tokens.front());
      command.line = line_number;
      for (std::size_t i = 1; i < tokens.size(); ++i) {
        command.args.push_back(std::move(tokens[i]));
      }
      commands.push_back(std::move(command));
      if (commands.size() > 1024) {
        return Result::fail(Status::limit, "too many commands");
      }
    }
    if (end == input.size()) break;
    begin = end + 1;
    ++line_number;
  }
  return Result::success();
}

}  // namespace vesper
