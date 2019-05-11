#include "lhat/named/alpha.h"

#include "lhat/named/sub.h"
#include "lhat/named/vars.h"

namespace lhat {
namespace named {
bool IsAlphaEquiv(const Term& m, const Term& n) {
  if (m.Type() != n.Type()) {
    return false;
  }
  return m.Match(
      [&n](const Abst& m_abst) -> bool {
        const Abst* n_abst = n.Get<Abst>();

        std::unordered_set<std::string> var_names;
        InsertVarNames(m_abst.Body(), &var_names);
        InsertVarNames(n_abst->Body(), &var_names);
        const std::string new_var_name = NewVarName(var_names);

        Term m_abst_body = m_abst.Body();
        Sub(m_abst.VarName(), Term(Var(new_var_name)), &m_abst_body);

        Term n_abst_body = n_abst->Body();
        Sub(n_abst->VarName(), Term(Var(new_var_name)), &n_abst_body);

        return IsAlphaEquiv(m_abst_body, n_abst_body);
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
