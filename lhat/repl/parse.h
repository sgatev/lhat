#ifndef LHAT_REPL_PARSE_H
#define LHAT_REPL_PARSE_H

#include <istream>
#include <string>

#include "lhat/util/error.h"

namespace lhat::repl {

// Parses input and returns the corresponding command name.
util::ErrorOr<std::string> ParseCommand(std::istream* input);

// Parses input and returns the corresponding constant name.
util::ErrorOr<std::string> ParseConstName(std::istream* input);

}  // namespace lhat::repl

#endif  // LHAT_REPL_PARSE_H
