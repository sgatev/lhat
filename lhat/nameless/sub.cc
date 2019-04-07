#include "sub.h"

namespace lhat {
namespace nameless {
namespace {
Term ShiftFreeVarIndex(const Term& term, int c, int d) {
  return term.Match(
      [c, d](const Abst& abst) -> Term {
        return Term(Abst(ShiftFreeVarIndex(abst.Body(), c + 1, d)));
      },
      [c, d](const Appl& appl) -> Term {
        return Term(Appl(ShiftFreeVarIndex(appl.Func(), c, d),
                         ShiftFreeVarIndex(appl.Arg(), c, d)));
      },
      [c, d](const Var& var) -> Term {
        if (var.Index() >= c) {
          return Term(Var(var.Index() + d));
        }
        return Term(Var(var.Index()));
      });
}
}  // namespace

Term ShiftFreeVarIndex(const Term& term, int d) {
  return ShiftFreeVarIndex(term, 0, d);
}

Term Sub(const Term& target, int idx, const Term& term) {
  return target.Match(
      [idx, &term](const Abst& abst) -> Term {
        return Term(
            Abst(Sub(abst.Body(), idx + 1, ShiftFreeVarIndex(term, 1))));
      },
      [idx, &term](const Appl& appl) -> Term {
        return Term(
            Appl(Sub(appl.Func(), idx, term), Sub(appl.Arg(), idx, term)));
      },
      [idx, &term](const Var& var) -> Term {
        if (var.Index() == idx) {
          return Term(term);
        }
        return Term(var);
      });
}
}  // namespace nameless
}  // namespace lhat
