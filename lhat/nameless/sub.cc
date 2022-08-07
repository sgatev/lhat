#include "lhat/nameless/sub.h"

namespace lhat::nameless {

void SubBoundVar(int idx, const Term& replacement, Term* target) {
  target->Match(
      [idx, replacement = replacement](Abst& abst) mutable {
        SubBoundVar(idx - 1, replacement, abst.MutableBody());
      },
      [idx, &replacement](Appl& appl) {
        SubBoundVar(idx, replacement, appl.MutableFunc());
        SubBoundVar(idx, replacement, appl.MutableArg());
      },
      [idx, &replacement, target](Var& var) {
        if (var.Index() == idx) {
          *target = replacement;
        }
      });
}

void SubFreeVar(int idx, const Term& replacement, Term* target) {
  target->Match(
      [idx, replacement](Abst& abst) mutable {
        SubFreeVar(idx, replacement, abst.MutableBody());
      },
      [idx, &replacement](Appl& appl) {
        SubFreeVar(idx, replacement, appl.MutableFunc());
        SubFreeVar(idx, replacement, appl.MutableArg());
      },
      [idx, &replacement, target](Var& var) {
        if (var.Index() == idx) {
          *target = replacement;
        }
      });
}

}  // namespace lhat::nameless
