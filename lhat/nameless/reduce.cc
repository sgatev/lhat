#include "reduce.h"

#include "sub.h"

namespace lhat {
namespace nameless {
namespace {
bool IsBetaRedex(const Appl& appl) { return appl.Func().Type() == ABST; }

void BetaReduce(int c, Term* term) {
  term->Match([](const Abst& abst) {},
              [c, term](Appl& appl) {
                if (IsBetaRedex(appl)) {
                  Term result = appl.Func().Get<Abst>()->Body();
                  Sub(-c, appl.Arg(), &result);
                  *term = result;
                }
              },
              [](const Var& var) {});
}

void BetaReduceAll(int c, Term* term) {
  term->Match([c](Abst& abst) { BetaReduceAll(c + 1, abst.MutableBody()); },
              [c, term](Appl& appl) {
                BetaReduceAll(c, appl.MutableFunc());
                BetaReduceAll(c, appl.MutableArg());
                BetaReduce(c, term);
              },
              [](const Var& var) {});
}
}  // namespace

void BetaReduce(Term* term) { BetaReduce(1, term); }

void BetaReduceAll(Term* term) { BetaReduceAll(1, term); }

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
