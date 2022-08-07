#include "lhat/repl/parse.h"

#include <cctype>

#include "lhat/util/parse.h"

namespace lhat::repl {
namespace {

bool IsCommandChar(char c) { return std::isalnum(c) || c == '-' || c == '?'; }

bool IsConstNameChar(char c) { return std::isalnum(c) || c == '*'; }

bool IsWhitespace(char c) { return c == ' ' || c == '\t'; }

}  // namespace

util::ErrorOr<std::string> ParseCommand(std::istream* input) {
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
  }

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

}  // namespace lhat::repl
