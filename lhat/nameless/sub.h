#ifndef LHAT_NAMELESS_SUB_H
#define LHAT_NAMELESS_SUB_H

#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Shifts the indexes of the free variables in term by d.
void ShiftFreeVarIndex(int d, Term* term);

// Substitutes the occurrences of idx in target with replacement.
void Sub(int idx, const Term& replacement, Term* target);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_SUB_H
