#ifndef LHAT_NAMELESS_SUB_H
#define LHAT_NAMELESS_SUB_H

#include "ast.h"

namespace lhat {
namespace nameless {
// ShiftFreeVarIndex shifts the indexes of the free variables in term by d.
std::shared_ptr<Term> ShiftFreeVarIndex(const std::shared_ptr<Term> term,
                                        int d);

// Sub substitutes the occurrences of idx in target with term.
std::shared_ptr<Term> Sub(const std::shared_ptr<Term> target, int idx,
                          const std::shared_ptr<Term> term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_SUB_H