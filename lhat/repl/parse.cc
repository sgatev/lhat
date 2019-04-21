#include "lhat/repl/parse.h"

#include <cctype>

namespace lhat {
namespace repl {
namespace {
bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

bool IsCommandChar(char c) { return std::isalnum(c) || c == '-' || c == '?'; }

bool IsConstNameChar(char c) { return std::isalnum(c) || c == '*'; }

void ParseWhitespace(std::istream* input) {
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
  }
}
}  // namespace

core::ParseResult<std::string> ParseCommand(std::istream* input) {
  ParseWhitespace(input);

  std::string name;
  while (!input->eof() && IsCommandChar(input->peek())) {
    name.push_back(input->get());
  }

  return name;
}

core::ParseResult<std::string> ParseConstName(std::istream* input) {
  ParseWhitespace(input);

  std::string name;
  while (!input->eof() && IsConstNameChar(input->peek())) {
    name.push_back(input->get());
  }

  return name;
}
}  // namespace repl
}  // namespace lhat
