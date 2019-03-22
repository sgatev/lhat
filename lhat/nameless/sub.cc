#include "sub.h"

namespace lhat {
namespace nameless {
namespace {
std::shared_ptr<Term> ShiftFreeVarIndex(const std::shared_ptr<Term> term, int c,
                                        int d) {
  if (term == nullptr) {
    return nullptr;
  }
  switch (term->Type()) {
    case ABST_TERM: {
      auto abst = std::static_pointer_cast<AbstTerm>(term);
      return AbstTerm::Make(ShiftFreeVarIndex(abst->body, c + 1, d));
    }
    case APPL_TERM: {
      auto appl = std::static_pointer_cast<ApplTerm>(term);
      return ApplTerm::Make(ShiftFreeVarIndex(appl->func, c, d),
                            ShiftFreeVarIndex(appl->arg, c, d));
    }
    case VAR_TERM: {
      auto var = std::static_pointer_cast<VarTerm>(term);
      if (var->idx >= c) {
        return VarTerm::Make(var->idx + d);
      }
      return VarTerm::Make(var->idx);
    }
  }
}
}  // namespace

std::shared_ptr<Term> ShiftFreeVarIndex(const std::shared_ptr<Term> term,
                                        int d) {
  return ShiftFreeVarIndex(term, 0, d);
}

std::shared_ptr<Term> Sub(const std::shared_ptr<Term> target, int idx,
                          const std::shared_ptr<Term> term) {
  if (target == nullptr) {
    return nullptr;
  }
  switch (target->Type()) {
    case ABST_TERM: {
      auto abst = std::static_pointer_cast<AbstTerm>(target);
      return AbstTerm::Make(
          Sub(abst->body, idx + 1, ShiftFreeVarIndex(term, 1)));
    }
    case APPL_TERM: {
      auto appl = std::static_pointer_cast<ApplTerm>(target);
      return ApplTerm::Make(Sub(appl->func, idx, term),
                            Sub(appl->arg, idx, term));
    }
    case VAR_TERM: {
      auto var = std::static_pointer_cast<VarTerm>(target);
      if (var->idx == idx) {
        return term;
      }
      return var;
    }
  }
  return nullptr;
}
}  // namespace nameless
}  // namespace lhat
