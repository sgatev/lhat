#ifndef LHAT_NAMED_PARSE_H
#define LHAT_NAMED_PARSE_H

#include <istream>

#include "lhat/named/ast.h"
#include "lhat/util/error.h"

namespace lhat {
namespace named {
// Parses expr and returns the corresponding AST.
util::ErrorOr<Term> Parse(std::istream* input);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PARSE_H
