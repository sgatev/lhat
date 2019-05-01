#include "lhat/nameless/eta.h"

namespace lhat {
namespace nameless {
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

        *term = body->Func();
        return true;
      },
      [](const Appl& appl) -> bool { return false; },
      [](const Var& var) -> bool { return false; });
}
}  // namespace nameless
}  // namespace lhat
