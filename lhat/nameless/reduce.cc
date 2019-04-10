#include "lhat/nameless/reduce.h"

#include "lhat/nameless/sub.h"

namespace lhat {
namespace nameless {
namespace {
bool IsBetaRedex(const Appl& appl) { return appl.Func().Type() == ABST; }
}  // namespace

bool BetaReduceTerm(Term* term) {
  return term->Match([](const Abst& abst) -> bool { return false; },
                     [term](Appl& appl) -> bool {
                       if (!IsBetaRedex(appl)) {
                         return false;
                       }
                       Term result = appl.Func().Get<Abst>()->Body();
                       Sub(-1, appl.Arg(), &result);
                       *term = result;
                       return true;
                     },
                     [](const Var& var) -> bool { return false; });
}

bool BetaReduceAppl(Term* term) {
  return term->Match(
      [](Abst& abst) -> bool { return BetaReduceAppl(abst.MutableBody()); },
      [term](Appl& appl) -> bool {
        return BetaReduceAppl(appl.MutableFunc()) ||
               BetaReduceAppl(appl.MutableArg()) || BetaReduceTerm(term);
      },
      [](const Var& var) -> bool { return false; });
}

bool IsBetaRedex(const Term& term) {
  return term.Type() == APPL && IsBetaRedex(*term.Get<Appl>());
}

bool IsNormalForm(const Term& term) {
  return term.Match(
      [](const Abst& abst) -> bool { return IsNormalForm(abst.Body()); },
      [](const Appl& appl) -> bool {
        return !IsBetaRedex(appl) && IsNormalForm(appl.Func()) &&
               IsNormalForm(appl.Arg());
      },
      [](const Var& var) -> bool { return true; });
}
}  // namespace nameless
}  // namespace lhat
