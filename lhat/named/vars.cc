#include "vars.h"

#include <deque>

namespace lhat {
namespace named {
void InsertBoundVarNames(const std::shared_ptr<Term> term,
                         std::unordered_set<std::string>* var_names) {
  if (term == nullptr) {
    return;
  }
  switch (term->Type()) {
    case ABST_TERM: {
      const auto abst = std::static_pointer_cast<AbstTerm>(term);
      var_names->insert(abst->var_name);
      InsertBoundVarNames(abst->body, var_names);
      break;
    }
    case APPL_TERM: {
      const auto appl = std::static_pointer_cast<ApplTerm>(term);
      InsertBoundVarNames(appl->func, var_names);
      InsertBoundVarNames(appl->arg, var_names);
      break;
    }
    case VAR_TERM:
      break;
  }
}

void InsertFreeVarNames(const std::shared_ptr<Term> term,
                        std::unordered_set<std::string>* var_names) {
  if (term == nullptr) {
    return;
  }
  switch (term->Type()) {
    case ABST_TERM: {
      const auto abst = std::static_pointer_cast<AbstTerm>(term);
      const bool should_erase_abst_var_name =
          var_names->find(abst->var_name) == var_names->end();
      InsertFreeVarNames(abst->body, var_names);
      if (should_erase_abst_var_name) {
        var_names->erase(abst->var_name);
      }
      break;
    }
    case APPL_TERM: {
      const auto appl = std::static_pointer_cast<ApplTerm>(term);
      InsertFreeVarNames(appl->func, var_names);
      InsertFreeVarNames(appl->arg, var_names);
      break;
    }
    case VAR_TERM:
      const auto var = std::static_pointer_cast<VarTerm>(term);
      var_names->insert(var->name);
      break;
  }
}

void InsertVarNames(const std::shared_ptr<Term> term,
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
