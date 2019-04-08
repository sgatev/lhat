#ifndef LHAT_NAMELESS_REDUCE_H
#define LHAT_NAMELESS_REDUCE_H

#include "ast.h"

namespace lhat {
namespace nameless {
// Performs a beta reduction on term if term is a beta redex, that is term is an
// application node and its function is an abstraction node.
void BetaReduceTerm(Term* term);

// Performs beta reductions on term and all of its descendants.
void BetaReduceSubTerms(Term* term);

// Returns true if there are no beta redexes in term.
bool IsNormalForm(const Term& term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_REDUCE_H
