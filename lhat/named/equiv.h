#ifndef LHAT_NAMED_EQUIV_H
#define LHAT_NAMED_EQUIV_H

#include "lhat/named/ast.h"

namespace lhat {
namespace named {
// AlphaEquiv returns true if m and n are simultaneously nullptr or
// alpha-equivalent.
bool AlphaEquiv(const Term& m, const Term& n);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_EQUIV_H
