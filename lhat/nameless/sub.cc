#include "lhat/nameless/sub.h"

namespace lhat {
namespace nameless {
namespace {
void ShiftFreeVarIndex(int c, int d, Term* term) {
  term->Match(
      [c, d](Abst& abst) { ShiftFreeVarIndex(c + 1, d, abst.MutableBody()); },
      [c, d](Appl& appl) {
        ShiftFreeVarIndex(c, d, appl.MutableFunc());
        ShiftFreeVarIndex(c, d, appl.MutableArg());
      },
      [c, d](Var& var) {
        if (var.Index() >= c) {
          var.SetIndex(var.Index() + d);
        }
      });
}
}  // namespace

void ShiftFreeVarIndex(int d, Term* term) { ShiftFreeVarIndex(0, d, term); }

void Sub(int idx, const Term& replacement, Term* target) {
  target->Match(
      [idx, replacement = replacement](Abst& abst) mutable {
        ShiftFreeVarIndex(1, &replacement);
        int next_idx;
        if (idx < 0) {
          // Substituting a bound var - index should decrease in abst.
          next_idx = idx - 1;
        } else {
          // Substituting a free var - index should increase in abst.
          next_idx = idx + 1;
        }
        Sub(next_idx, replacement, abst.MutableBody());
      },
      [idx, &replacement](Appl& appl) {
        Sub(idx, replacement, appl.MutableFunc());
        Sub(idx, replacement, appl.MutableArg());
      },
      [idx, &replacement, target](Var& var) {
        if (var.Index() == idx) {
          *target = replacement;
        }
      });
}
}  // namespace nameless
}  // namespace lhat
