#ifndef LHAT_NAMED_PARSER_H
#define LHAT_NAMED_PARSER_H

#include "lhat/core/parse.h"
#include "lhat/named/ast.h"

#include "absl/strings/string_view.h"

namespace lhat {
namespace named {
// Parse parses expr and returns the corresponding AST.
core::ParseResult<Term> Parse(const absl::string_view expr);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PARSER_H
