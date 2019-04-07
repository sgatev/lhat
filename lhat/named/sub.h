#ifndef LHAT_NAMED_SUB_H
#define LHAT_NAMED_SUB_H

#include <string>

#include "ast.h"

namespace lhat {
namespace named {
// SafeSub substitutes the occurrences of var_name in target with replacement
// and performs the necessary bound variable renames in target in order to avoid
// unintentional capturing variables that are free in replacement.
void SafeSub(const std::string& var_name, const Term& replacement,
             Term* target);

// Sub substitutes the occurrences of var_name in target with replacement.
void Sub(const std::string& var_name, const Term& replacement, Term* target);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_SUB_H
