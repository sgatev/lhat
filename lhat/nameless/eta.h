#ifndef LHAT_NAMELESS_ETA_H
#define LHAT_NAMELESS_ETA_H

#include "lhat/nameless/ast.h"

namespace lhat::nameless {

// Performs eta reduction on term.
bool EtaReduceTerm(Term* term);

// Performs eta reduction on term and its subterms.
bool EtaReduceSubTerms(Term* term);

}  // namespace lhat::nameless

#endif  // LHAT_NAMELESS_ETA_H
