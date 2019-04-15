#ifndef LHAT_NAMELESS_BETA_H
#define LHAT_NAMELESS_BETA_H

#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Performs a beta reduction on term if term is a beta redex, that is term is an
// application node and its function is an abstraction node.
bool BetaReduceTerm(Term* term);

// Performs applicative beta reduction on term and all of its subterms.
bool BetaReduceAppl(Term* term);

// Performs normal beta reduction on term and all of its subterms.
bool BetaReduceNormal(Term* term);

// Returns true if term is a beta redex.
bool IsBetaRedex(const Term& term);

// Returns true if there are no beta redexes in term.
bool IsBetaNormalForm(const Term& term);

// Returns true if the head position in term is not a redex.
bool IsHeadNormalForm(const Term& term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_BETA_H
