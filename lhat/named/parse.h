#ifndef LHAT_NAMED_PARSE_H
#define LHAT_NAMED_PARSE_H

#include <istream>

#include "lhat/core/parse.h"
#include "lhat/named/ast.h"

namespace lhat {
namespace named {
// Parses expr and returns the corresponding AST.
core::ParseResult<Term> Parse(std::istream* input);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PARSE_H
