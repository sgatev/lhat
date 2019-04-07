#ifndef LHAT_NAMELESS_SUB_H
#define LHAT_NAMELESS_SUB_H

#include "ast.h"

namespace lhat {
namespace nameless {
// ShiftFreeVarIndex shifts the indexes of the free variables in term by d.
Term ShiftFreeVarIndex(const Term& term, int d);

// Sub substitutes the occurrences of idx in target with term.
Term Sub(const Term& target, int idx, const Term& term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_SUB_H
