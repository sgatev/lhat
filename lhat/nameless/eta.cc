#include "lhat/nameless/eta.h"

namespace lhat {
namespace nameless {
namespace {
bool HasBoundVarIndex(const Term& term, int idx) {
  return term.Match(
      [idx](const Abst& abst) -> bool {
        return HasBoundVarIndex(abst.Body(), idx - 1);
      },
      [idx](const Appl& appl) -> bool {
        return HasBoundVarIndex(appl.Func(), idx) ||
               HasBoundVarIndex(appl.Arg(), idx);
      },
      [idx](const Var& var) -> bool { return var.Index() == idx; });
}
}  // namespace

bool EtaReduceTerm(Term* term) {
  return term->Match(
      [term](const Abst& abst) -> bool {
        if (abst.Body().Type() != APPL) {
          return false;
        }

        const Appl* body = abst.Body().Get<Appl>();
        if (body->Arg().Type() != VAR) {
          return false;
        }

        const Var* arg = body->Arg().Get<Var>();
        if (arg->Index() != -1) {
          return false;
        }

        if (HasBoundVarIndex(body->Func(), -1)) {
          return false;
        }

        Term result = body->Func();
        *term = result;
        return true;
      },
      [](const Appl& appl) -> bool { return false; },
      [](const Var& var) -> bool { return false; });
}

bool EtaReduceSubTerms(Term* term) {
  return term->Match(
      [term](Abst& abst) -> bool {
        return EtaReduceTerm(term) || EtaReduceSubTerms(abst.MutableBody());
      },
      [](Appl& appl) -> bool {
        return EtaReduceSubTerms(appl.MutableFunc()) ||
               EtaReduceSubTerms(appl.MutableArg());
      },
      [](const Var& var) -> bool { return false; });
}
}  // namespace nameless
}  // namespace lhat
