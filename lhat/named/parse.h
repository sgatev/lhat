#ifndef LHAT_NAMED_PARSE_H
#define LHAT_NAMED_PARSE_H

#include <istream>

#include "lhat/named/ast.h"
#include "lhat/util/error.h"

namespace lhat::named {

// Parses expr and returns the corresponding AST.
util::ErrorOr<Term> Parse(std::istream* input);

}  // namespace lhat::named

#endif  // LHAT_NAMED_PARSE_H
