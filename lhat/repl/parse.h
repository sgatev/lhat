#ifndef LHAT_REPL_PARSE_H
#define LHAT_REPL_PARSE_H

#include <istream>
#include <string>

#include "lhat/core/parse.h"

namespace lhat {
namespace repl {
// Parses input and returns the corresponding command name.
core::ParseResult<std::string> ParseCommand(std::istream* input);

// Parses input and returns the corresponding constant name.
core::ParseResult<std::string> ParseConstName(std::istream* input);
}  // namespace repl
}  // namespace lhat

#endif  // LHAT_REPL_PARSE_H
