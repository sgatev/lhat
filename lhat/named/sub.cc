#include "lhat/named/sub.h"

#include <unordered_set>

#include "lhat/named/vars.h"

namespace lhat::named {

void SafeSub(const std::string& var_name, const Term& replacement,
             Term* target) {
  target->Match(
      [&var_name, &replacement](Abst& abst) {
        if (abst.VarName() == var_name) {
          // no need to substitute in this case
          return;
        }

        std::unordered_set<std::string> body_free_var_names;
        InsertFreeVarNames(abst.Body(), &body_free_var_names);

        if (body_free_var_names.find(var_name) == body_free_var_names.end()) {
          // no need to substitute in this case
          return;
        }

        std::unordered_set<std::string> replacement_free_var_names;
        InsertFreeVarNames(replacement, &replacement_free_var_names);

        if (replacement_free_var_names.find(abst.VarName()) !=
            replacement_free_var_names.end()) {
          // rename the variable name in order to ensure collision-free
          // substitution
          std::unordered_set<std::string> var_names;
          InsertVarNames(abst.Body(), &var_names);
          InsertVarNames(replacement, &var_names);
          const std::string new_var_name = NewVarName(var_names);

          Sub(abst.VarName(), Term(Var(new_var_name)), abst.MutableBody());
          abst.SetVarName(new_var_name);
        }

        SafeSub(var_name, replacement, abst.MutableBody());
      },
      [&var_name, &replacement](Appl& appl) {
        SafeSub(var_name, replacement, appl.MutableFunc());
        SafeSub(var_name, replacement, appl.MutableArg());
      },
      [&var_name, &replacement, target](const Var& var) {
        if (var.Name() == var_name) {
          *target = replacement;
        }
      });
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

}  // namespace lhat::named
