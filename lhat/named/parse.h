#ifndef LHAT_NAMED_PARSE_H
#define LHAT_NAMED_PARSE_H

#include <string>

#include "lhat/core/parse.h"
#include "lhat/named/ast.h"

namespace lhat {
namespace named {
// Parses expr and returns the corresponding AST.
core::ParseResult<Term> Parse(const std::string_view expr);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PARSE_H
