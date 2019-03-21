#ifndef LHAT_NAMELESS_SHIFT_H
#define LHAT_NAMELESS_SHIFT_H

#include "ast.h"

namespace lhat {
namespace nameless {
// ShiftFreeVarIndex shifts the indexes of the free variables in term by d.
void ShiftFreeVarIndex(int d, std::shared_ptr<Term> term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_SHIFT_H
