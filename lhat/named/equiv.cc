#include "equiv.h"

#include "sub.h"

namespace lhat {
namespace named {
bool AlphaEquiv(const std::shared_ptr<Term> m, const std::shared_ptr<Term> n) {
  if (m == nullptr && n == nullptr) {
    return true;
  }
  if (m == nullptr || n == nullptr) {
    return false;
  }
  if (m->Type() != n->Type()) {
    return false;
  }
  switch (m->Type()) {
    case ABST_TERM: {
      const auto m_abst = std::static_pointer_cast<AbstTerm>(m);
      const auto n_abst = std::static_pointer_cast<AbstTerm>(n);
      return AlphaEquiv(m_abst->body,
                        Sub(n_abst->body, n_abst->var_name,
                            std::make_shared<VarTerm>(m_abst->var_name)));
    }
    case APPL_TERM: {
      const auto m_appl = std::static_pointer_cast<ApplTerm>(m);
      const auto n_appl = std::static_pointer_cast<ApplTerm>(n);
      return AlphaEquiv(m_appl->func, n_appl->func) &&
             AlphaEquiv(m_appl->arg, n_appl->arg);
    }
    case VAR_TERM: {
      const auto m_var = std::static_pointer_cast<VarTerm>(m);
      const auto n_var = std::static_pointer_cast<VarTerm>(n);
      return m_var->name == n_var->name;
    }
  }
}
}  // namespace named
}  // namespace lhat
