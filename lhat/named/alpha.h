#ifndef LHAT_NAMED_ALPHA_H
#define LHAT_NAMED_ALPHA_H

#include "lhat/named/ast.h"

namespace lhat::named {

// Returns true if the terms m and n are alpha-equivalent.
bool IsAlphaEquiv(const Term& m, const Term& n);

}  // namespace lhat::named

#endif  // LHAT_NAMED_ALPHA_H
