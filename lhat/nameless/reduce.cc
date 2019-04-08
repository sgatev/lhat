#include "reduce.h"

#include "sub.h"

namespace lhat {
namespace nameless {
namespace {
bool IsBetaRedex(const Appl& appl) { return appl.Func().Type() == ABST; }
}  // namespace

void BetaReduce(Term* term) {
  term->Match([](const Abst& abst) {},
              [term](Appl& appl) {
                if (IsBetaRedex(appl)) {
                  ShiftFreeVarIndex(1, appl.MutableArg());
                  Term result = appl.Func().Get<Abst>()->Body();
                  Sub(-1, appl.Arg(), &result);
                  ShiftFreeVarIndex(-1, &result);
                  *term = result;
                }
              },
              [](const Var& var) {});
}

void BetaReduceAll(Term* term) {
  term->Match([](Abst& abst) { BetaReduceAll(abst.MutableBody()); },
              [term](Appl& appl) {
                BetaReduceAll(appl.MutableFunc());
                BetaReduceAll(appl.MutableArg());
                BetaReduce(term);
              },
              [](const Var& var) {});
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
