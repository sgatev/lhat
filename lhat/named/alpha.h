#ifndef LHAT_NAMED_ALPHA_H
#define LHAT_NAMED_ALPHA_H

#include "lhat/named/ast.h"

namespace lhat {
namespace named {
// Returns true if the terms m and n are alpha-equivalent.
bool IsAlphaEquiv(const Term& m, const Term& n);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_ALPHA_H
