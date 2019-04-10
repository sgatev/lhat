#ifndef LHAT_NAMELESS_REDUCE_H
#define LHAT_NAMELESS_REDUCE_H

#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Performs a beta reduction on term if term is a beta redex, that is term is an
// application node and its function is an abstraction node.
bool BetaReduceTerm(Term* term);

// Performs applicative beta reduction on term and all of its subterms.
bool BetaReduceAppl(Term* term);

// Returns true if there are no beta redexes in term.
bool IsNormalForm(const Term& term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_REDUCE_H
