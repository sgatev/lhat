#include "sub.h"

#include <unordered_set>

#include "vars.h"

namespace lhat {
namespace named {
namespace {
void RenameBoundVar(const std::string& from, const std::string& to,
                    Term* term) {
  return term->Match(
      [&from, &to](Abst& abst) {
        if (abst.VarName() == from) {
          abst.SetVarName(to);
          Sub(from, Term(Var(to)), abst.MutableBody());
          RenameBoundVar(from, to, abst.MutableBody());
        } else {
          RenameBoundVar(from, to, abst.MutableBody());
        }
      },
      [&from, &to](Appl& appl) {
        RenameBoundVar(from, to, appl.MutableFunc());
        RenameBoundVar(from, to, appl.MutableArg());
      },
      [](const Var& var) {});
}
}  // namespace

void SafeSub(const std::string& var_name, const Term& replacement,
             Term* target) {
  std::unordered_set<std::string> bound_var_names;
  InsertBoundVarNames(*target, &bound_var_names);

  std::unordered_set<std::string> free_var_names;
  InsertFreeVarNames(*target, &free_var_names);
  InsertFreeVarNames(replacement, &free_var_names);

  std::unordered_set<std::string> conflicting_var_names;
  for (const std::string& var_name : free_var_names) {
    if (bound_var_names.find(var_name) != bound_var_names.end()) {
      conflicting_var_names.insert(var_name);
    }
  }

  for (const std::string& conflicting_var_name : conflicting_var_names) {
    RenameBoundVar(conflicting_var_name, NewVarName(free_var_names), target);
  }

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
