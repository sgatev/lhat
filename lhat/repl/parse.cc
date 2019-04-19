#include "lhat/repl/parse.h"

#include <cctype>

namespace lhat {
namespace repl {
namespace {
bool IsWhitespace(char c) { return c == ' ' || c == '\t'; }

bool IsCommandChar(char c) { return std::isalnum(c) || c == '-' || c == '?'; }

int ParseWhitespace(io::CharReader* reader) {
  int idx = 0;
  while (!reader->Empty() && IsWhitespace(reader->Peek())) {
    reader->Next();
    idx++;
  }
  return idx;
}
}  // namespace

core::ParseResult<std::string> ParseCommand(io::CharReader* reader) {
  int idx = ParseWhitespace(reader);

  std::string name;
  while (!reader->Empty() && IsCommandChar(reader->Peek())) {
    name.push_back(reader->Peek());
    reader->Next();
    idx++;
  }

  return core::ParseResult<std::string>(idx, name);
}
}  // namespace repl
}  // namespace lhat
