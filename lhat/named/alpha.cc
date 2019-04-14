#include "lhat/named/alpha.h"

#include "lhat/named/sub.h"

namespace lhat {
namespace named {
bool IsAlphaEquiv(const Term& m, const Term& n) {
  if (m.Type() != n.Type()) {
    return false;
  }
  return m.Match(
      [&n](const Abst& m_abst) -> bool {
        const Abst* n_abst = n.Get<Abst>();
        Term abst_body = n_abst->Body();
        Sub(n_abst->VarName(), Term(Var(m_abst.VarName())), &abst_body);
        return IsAlphaEquiv(m_abst.Body(), abst_body);
      },
      [&n](const Appl& m_appl) -> bool {
        const Appl* n_appl = n.Get<Appl>();
        return IsAlphaEquiv(m_appl.Func(), n_appl->Func()) &&
               IsAlphaEquiv(m_appl.Arg(), n_appl->Arg());
      },
      [&n](const Var& m_var) -> bool {
        const Var* n_var = n.Get<Var>();
        return m_var.Name() == n_var->Name();
      });
}
}  // namespace named
}  // namespace lhat
