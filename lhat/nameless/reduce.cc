#include "reduce.h"

#include "sub.h"

namespace lhat {
namespace nameless {
namespace {
bool IsBetaRedex(const Appl& appl) { return appl.Func().Type() == ABST; }

void BetaReduceTerm(int c, Term* term) {
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

void BetaReduceSubTerms(int c, Term* term) {
  term->Match(
      [c](Abst& abst) { BetaReduceSubTerms(c + 1, abst.MutableBody()); },
      [c, term](Appl& appl) {
        BetaReduceSubTerms(c, appl.MutableFunc());
        BetaReduceSubTerms(c, appl.MutableArg());
        BetaReduceTerm(c, term);
      },
      [](const Var& var) {});
}
}  // namespace

void BetaReduceTerm(Term* term) { BetaReduceTerm(1, term); }

void BetaReduceSubTerms(Term* term) { BetaReduceSubTerms(1, term); }

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
