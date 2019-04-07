#ifndef LHAT_NAMED_SUB_H
#define LHAT_NAMED_SUB_H

#include <string>

#include "ast.h"

namespace lhat {
namespace named {
// SafeSub substitutes the occurrences of var_name in target with replacement
// and performs the necessary bound variable renames in target in order to avoid
// unintentional capturing variables that are free in replacement.
Term SafeSub(const Term& target, const std::string& var_name,
             const Term& replacement);

// Sub substitutes the occurrences of var_name in target with replacement.
Term Sub(const Term& target, const std::string& var_name,
         const Term& replacement);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_SUB_H
