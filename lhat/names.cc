#include "names.h"

namespace lhat {
namespace {
nameless::Term StripNamesFromTerm(
    const named::Term& term, NameContext* nctx,
    std::unordered_map<std::string, int>* abst_var_names, int abst_count) {
  return term.Match(
      [nctx, abst_var_names,
       abst_count](const named::Abst& abst) -> nameless::Term {
        int old_abst_var_name_idx = -1;
        if (abst_var_names->find(abst.VarName()) != abst_var_names->end()) {
          old_abst_var_name_idx = abst_var_names->at(abst.VarName());
        }
        abst_var_names->insert({abst.VarName(), abst_count});

        const nameless::Term body = StripNamesFromTerm(
            abst.Body(), nctx, abst_var_names, abst_count + 1);

        if (old_abst_var_name_idx >= 0) {
          abst_var_names->at(abst.VarName()) = old_abst_var_name_idx;
        } else {
          abst_var_names->erase(abst.VarName());
        }

        return nameless::Abst(body);
      },
      [nctx, abst_var_names,
       abst_count](const named::Appl& appl) -> nameless::Term {
        return nameless::Appl(
            StripNamesFromTerm(appl.Func(), nctx, abst_var_names, abst_count),
            StripNamesFromTerm(appl.Arg(), nctx, abst_var_names, abst_count));
      },
      [nctx, abst_var_names](const named::Var& var) -> nameless::Term {
        if (abst_var_names->find(var.Name()) != abst_var_names->end()) {
          return nameless::Var(-abst_var_names->at(var.Name()) - 1);
        }
        if (!nctx->HasName(var.Name())) {
          nctx->AddName(var.Name());
        }
        return nameless::Var(nctx->GetIndexForName(var.Name()));
      });
}
}  // namespace

int NameContext::AddName(const std::string& name) {
  const auto name_idx = name_to_idx_.find(name);
  if (name_idx != name_to_idx_.end()) {
    // Name is already added.
    return name_idx->second;
  }

  int idx = names_.size();
  name_to_idx_[name] = idx;
  names_.push_back(name);
  return idx;
}

std::string NameContext::GetNameForIndex(int idx) const { return names_[idx]; }

int NameContext::GetIndexForName(const std::string& name) const {
  return name_to_idx_.at(name);
}

bool NameContext::HasName(const std::string& name) const {
  return name_to_idx_.find(name) != name_to_idx_.end();
}

nameless::Term StripNames(const named::Term& term) {
  NameContext nctx;
  std::unordered_map<std::string, int> abst_var_names;
  return StripNamesFromTerm(term, &nctx, &abst_var_names, 0);
}
}  // namespace lhat
