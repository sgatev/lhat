#ifndef LHAT_REPL_PARSE_H
#define LHAT_REPL_PARSE_H

#include <istream>
#include <string>

#include "lhat/core/parse.h"

namespace lhat {
namespace repl {
// Parses expr and returns the corresponding command name.
core::ParseResult<std::string> ParseCommand(std::istream* input);
}  // namespace repl
}  // namespace lhat

#endif  // LHAT_REPL_PARSE_H
