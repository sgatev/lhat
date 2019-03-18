#ifndef LHAT_SUBST_H
#define LHAT_SUBST_H

#include <string>

#include "ast.h"

namespace lhat {
// SafeSub substitutes the occurrences of var_name in target with term and
// performs the necessary bound variable renames in target in order to avoid
// unintentional capturing variables that are free in term.
std::shared_ptr<Term> SafeSub(const std::shared_ptr<Term> target,
                              const std::string& var_name,
                              const std::shared_ptr<Term> term);

// Sub substitutes the occurrences of var_name in target with term.
std::shared_ptr<Term> Sub(const std::shared_ptr<Term> target,
                          const std::string& var_name,
                          const std::shared_ptr<Term> term);
}  // namespace lhat

#endif  // LHAT_SUBST_H
