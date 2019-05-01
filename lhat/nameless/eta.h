#ifndef LHAT_NAMELESS_ETA_H
#define LHAT_NAMELESS_ETA_H

#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Performs eta reduction on term.
bool EtaReduceTerm(Term* term);
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_ETA_H
