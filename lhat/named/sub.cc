#include "sub.h"

#include <unordered_set>

#include "vars.h"

namespace lhat {
namespace named {
namespace {
Term RenameBoundVarInTerm(const Term& term, const std::string& from,
                          const std::string& to) {
  return term.Match(
      [&from, &to](const Abst& abst) -> Term {
        if (abst.VarName() == from) {
          Term body = abst.Body();
          Sub(from, Term(Var(to)), &body);
          return Term(Abst(to, body));
        } else {
          return Term(Abst(abst.VarName(),
                           RenameBoundVarInTerm(abst.Body(), from, to)));
        }
      },
      [&from, &to](const Appl& appl) -> Term {
        return Term(Appl(RenameBoundVarInTerm(appl.Func(), from, to),
                         RenameBoundVarInTerm(appl.Arg(), from, to)));
      },
      [](const Var& var) -> Term { return Term(Var(var.Name())); });
}
}  // namespace

void SafeSub(const std::string& var_name, const Term& replacement,
             Term* target) {
  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(*target, &var_names);
  InsertFreeVarNames(replacement, &var_names);
  std::string new_var_name = NewVarName(var_names);
  return Sub(var_name, replacement, target);
}

void Sub(const std::string& var_name, const Term& replacement, Term* target) {
  target->Match(
      [&var_name, &replacement](Abst& abst) {
        if (abst.VarName() != var_name) {
          Sub(var_name, replacement, abst.MutableBody());
        }
      },
      [&var_name, &replacement](Appl& appl) {
        Sub(var_name, replacement, appl.MutableFunc());
        Sub(var_name, replacement, appl.MutableArg());
      },
      [&var_name, &replacement, target](const Var& var) {
        if (var.Name() == var_name) {
          *target = replacement;
        }
      });
}
}  // namespace named
}  // namespace lhat
