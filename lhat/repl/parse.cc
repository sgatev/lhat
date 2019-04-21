#include "lhat/repl/parse.h"

#include <cctype>

namespace lhat {
namespace repl {
namespace {
bool IsCommandChar(char c) { return std::isalnum(c) || c == '-' || c == '?'; }

bool IsConstNameChar(char c) { return std::isalnum(c) || c == '*'; }
}  // namespace

core::ParseResult<std::string> ParseCommand(std::istream* input) {
  core::DiscardWhitespace(input);

  std::string name;
  while (!input->eof() && IsCommandChar(input->peek())) {
    name.push_back(input->get());
  }

  return name;
}

core::ParseResult<std::string> ParseConstName(std::istream* input) {
  core::DiscardWhitespace(input);

  std::string name;
  while (!input->eof() && IsConstNameChar(input->peek())) {
    name.push_back(input->get());
  }

  return name;
}
}  // namespace repl
}  // namespace lhat
