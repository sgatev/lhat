#include "shift.h"

namespace lhat {
namespace nameless {
namespace {
void ShiftFreeVarIndex(int c, int d, std::shared_ptr<Term> term) {
  if (term == nullptr) {
    return;
  }
  switch (term->Type()) {
    case ABST_TERM: {
      auto abst = std::static_pointer_cast<AbstTerm>(term);
      ShiftFreeVarIndex(c + 1, d, abst->body);
      break;
    }
    case APPL_TERM: {
      auto appl = std::static_pointer_cast<ApplTerm>(term);
      ShiftFreeVarIndex(c, d, appl->func);
      ShiftFreeVarIndex(c, d, appl->arg);
      break;
    }
    case VAR_TERM: {
      auto var = std::static_pointer_cast<VarTerm>(term);
      if (var->idx >= c) {
        var->idx += d;
      }
      break;
    }
  }
}
}  // namespace

void ShiftFreeVarIndex(int d, std::shared_ptr<Term> term) {
  ShiftFreeVarIndex(0, d, term);
}
}  // namespace nameless
}  // namespace lhat
