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
          return Term(Abst(to, Sub(abst.Body(), from, Term(Var(to)))));
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

Term SafeSub(const Term& target, const std::string& var_name,
             const Term& replacement) {
  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(target, &var_names);
  InsertFreeVarNames(replacement, &var_names);
  std::string new_var_name = NewVarName(var_names);
  return Sub(target, var_name, replacement);
}

Term Sub(const Term& target, const std::string& var_name,
         const Term& replacement) {
  return target.Match(
      [&var_name, &replacement, &target](const Abst& abst) -> Term {
        if (abst.VarName() == var_name) {
          return target;
        }
        return Term(
            Abst(abst.VarName(), Sub(abst.Body(), var_name, replacement)));
      },
      [&var_name, &replacement](const Appl& appl) -> Term {
        return Term(Appl(Sub(appl.Func(), var_name, replacement),
                         Sub(appl.Arg(), var_name, replacement)));
      },
      [&var_name, &replacement, &target](const Var& var) -> Term {
        if (var.Name() == var_name) {
          return replacement;
        } else {
          return target;
        }
      });
}
}  // namespace named
}  // namespace lhat
