#ifndef LHAT_REPL_PARSE_H
#define LHAT_REPL_PARSE_H

#include <string>

#include "lhat/core/parse.h"
#include "lhat/io/char_reader.h"

namespace lhat {
namespace repl {
// Parses expr and returns the corresponding command name.
core::ParseResult<std::string> ParseCommand(io::CharReader* reader);
}  // namespace repl
}  // namespace lhat

#endif  // LHAT_REPL_PARSE_H
