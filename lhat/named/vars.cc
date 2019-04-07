#include "vars.h"

#include <deque>

namespace lhat {
namespace named {
void InsertBoundVarNames(const Term& term,
                         std::unordered_set<std::string>* var_names) {
  term.Match(
      [var_names](const Abst& abst) {
        var_names->insert(abst.VarName());
        InsertBoundVarNames(abst.Body(), var_names);
      },
      [var_names](const Appl& appl) {
        InsertBoundVarNames(appl.Func(), var_names);
        InsertBoundVarNames(appl.Arg(), var_names);
      },
      [](const Var& var) {});
}

void InsertFreeVarNames(const Term& term,
                        std::unordered_set<std::string>* var_names) {
  term.Match(
      [var_names](const Abst& abst) {
        const bool should_erase_abst_var_name =
            var_names->find(abst.VarName()) == var_names->end();
        InsertFreeVarNames(abst.Body(), var_names);
        if (should_erase_abst_var_name) {
          var_names->erase(abst.VarName());
        }
      },
      [var_names](const Appl& appl) {
        InsertFreeVarNames(appl.Func(), var_names);
        InsertFreeVarNames(appl.Arg(), var_names);
      },
      [var_names](const Var& var) { var_names->insert(var.Name()); });
}

void InsertVarNames(const Term& term,
                    std::unordered_set<std::string>* var_names) {
  InsertFreeVarNames(term, var_names);
  InsertBoundVarNames(term, var_names);
}

std::string NewVarName(const std::unordered_set<std::string>& var_names) {
  std::deque<char> name_chars;
  while (true) {
    if (!name_chars.empty() && name_chars.back() == 'z') {
      int count = 0;
      while (!name_chars.empty() && name_chars.back() == 'z') {
        count++;
        name_chars.pop_back();
      }
      if (name_chars.empty()) {
        name_chars.push_back('a');
      } else {
        char c = name_chars.back();
        name_chars.pop_back();
        name_chars.push_back(c + 1);
      }
      for (int i = 0; i < count; i++) {
        name_chars.push_back('a');
      }
    } else {
      if (name_chars.empty()) {
        name_chars.push_back('a');
      } else {
        char c = name_chars.back();
        name_chars.pop_back();
        name_chars.push_back(c + 1);
      }
    }

    std::string name(name_chars.begin(), name_chars.end());
    if (var_names.find(name) == var_names.end()) {
      return name;
    }
  }
}
}  // namespace named
}  // namespace lhat
