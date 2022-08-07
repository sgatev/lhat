#ifndef LHAT_NAMELESS_SUB_H
#define LHAT_NAMELESS_SUB_H

#include "lhat/nameless/ast.h"

namespace lhat::nameless {

// Substitutes the occurrences of the bound variable indicated by idx in target
// with replacement.
void SubBoundVar(int idx, const Term& replacement, Term* target);

// Substitutes the occurrences of the free variable indicated by idx in target
// with replacement.
void SubFreeVar(int idx, const Term& replacement, Term* target);

}  // namespace lhat::nameless

#endif  // LHAT_NAMELESS_SUB_H
