#include "lhat/repl/parse.h"

#include <cctype>

#include "lhat/util/parse.h"

namespace lhat {
namespace repl {
namespace {
bool IsCommandChar(char c) { return std::isalnum(c) || c == '-' || c == '?'; }

bool IsConstNameChar(char c) { return std::isalnum(c) || c == '*'; }
}  // namespace

util::ErrorOr<std::string> ParseCommand(std::istream* input) {
  util::DiscardWhitespace(input);

  std::string name;
  while (!input->eof() && IsCommandChar(input->peek())) {
    name.push_back(input->get());
  }

  return name;
}

util::ErrorOr<std::string> ParseConstName(std::istream* input) {
  util::DiscardWhitespace(input);

  std::string name;
  while (!input->eof() && IsConstNameChar(input->peek())) {
    name.push_back(input->get());
  }

  return name;
}
}  // namespace repl
}  // namespace lhat
