#ifndef LHAT_NAMED_PARSE_H
#define LHAT_NAMED_PARSE_H

#include "lhat/core/parse.h"
#include "lhat/io/char_reader.h"
#include "lhat/named/ast.h"

namespace lhat {
namespace named {
// Parses expr and returns the corresponding AST.
core::ParseResult<Term> Parse(io::CharReader* reader);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PARSE_H
