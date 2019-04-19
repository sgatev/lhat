#include "lhat/repl/parse.h"

#include <cctype>

namespace lhat {
namespace repl {
namespace {
int ParseWhitespace(const std::string_view expr) {
  int idx = 0;
  while (idx < expr.size() && (expr[idx] == ' ' || expr[idx] == '\t')) {
    idx++;
  }
  return idx;
}
}  // namespace

core::ParseResult<std::string> ParseCommand(const std::string_view expr) {
  int idx = ParseWhitespace(expr);
  std::string name;
  while (idx < expr.size() &&
         (std::isalnum(expr[idx]) || expr[idx] == '-' || expr[idx] == '?')) {
    name.push_back(expr[idx]);
    idx++;
  }
  return core::ParseResult<std::string>(idx, name);
}
}  // namespace repl
}  // namespace lhat
