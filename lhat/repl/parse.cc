#include "lhat/repl/parse.h"

#include <cctype>

namespace lhat {
namespace repl {
namespace {
bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

bool IsCommandChar(char c) { return std::isalnum(c) || c == '-' || c == '?'; }

bool IsConstNameChar(char c) { return std::isalnum(c) || c == '*'; }

int ParseWhitespace(std::istream* input) {
  int idx = 0;
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
    idx++;
  }
  return idx;
}
}  // namespace

core::ParseResult<std::string> ParseCommand(std::istream* input) {
  int idx = ParseWhitespace(input);

  std::string name;
  while (!input->eof() && IsCommandChar(input->peek())) {
    name.push_back(input->get());
    idx++;
  }

  return core::ParseResult<std::string>(idx, name);
}

core::ParseResult<std::string> ParseConstName(std::istream* input) {
  int idx = ParseWhitespace(input);

  std::string name;
  while (!input->eof() && IsConstNameChar(input->peek())) {
    name.push_back(input->get());
    idx++;
  }

  return core::ParseResult<std::string>(idx, name);
}
}  // namespace repl
}  // namespace lhat
