#include "lhat/transform/names.h"

#include "lhat/named/vars.h"

namespace lhat {
namespace transform {
namespace {
named::Term AddNames(const nameless::Term& term, NameContext* free_nctx,
                     NameContext* bound_nctx,
                     std::unordered_set<std::string>* names, int abst_count) {
  return term.Match(
      [free_nctx, bound_nctx, names,
       abst_count](const nameless::Abst& abst) -> named::Term {
        if (!bound_nctx->HasIndex(abst_count)) {
          std::string name = named::NewVarName(*names);
          bound_nctx->SetName(name, abst_count);
          names->insert(name);
        }
        return named::Abst(bound_nctx->GetNameForIndex(abst_count),
                           AddNames(abst.Body(), free_nctx, bound_nctx, names,
                                    abst_count + 1));
      },
      [free_nctx, bound_nctx, names,
       abst_count](const nameless::Appl& appl) -> named::Term {
        named::Term func =
            AddNames(appl.Func(), free_nctx, bound_nctx, names, abst_count);
        named::Term arg =
            AddNames(appl.Arg(), free_nctx, bound_nctx, names, abst_count);
        return named::Appl(func, arg);
      },
      [free_nctx, bound_nctx, names,
       abst_count](const nameless::Var& var) -> named::Term {
        if (var.Index() < 0) {
          return named::Var(
              bound_nctx->GetNameForIndex(abst_count + var.Index()));
        }
        if (!free_nctx->HasIndex(var.Index())) {
          std::string name = named::NewVarName(*names);
          free_nctx->SetName(name, var.Index());
          names->insert(name);
        }
        return named::Var(free_nctx->GetNameForIndex(var.Index()));
      });
}

nameless::Term RemoveNames(const named::Term& term, NameContext* free_nctx,
                           std::unordered_map<std::string, int>* abst_var_names,
                           int abst_count) {
  return term.Match(
      [free_nctx, abst_var_names,
       abst_count](const named::Abst& abst) -> nameless::Term {
        int old_abst_var_name_idx = -1;
        if (abst_var_names->find(abst.VarName()) != abst_var_names->end()) {
          old_abst_var_name_idx = abst_var_names->at(abst.VarName());
        }
        abst_var_names->insert({abst.VarName(), abst_count});

        const nameless::Term body =
            RemoveNames(abst.Body(), free_nctx, abst_var_names, abst_count + 1);

        if (old_abst_var_name_idx >= 0) {
          abst_var_names->at(abst.VarName()) = old_abst_var_name_idx;
        } else {
          abst_var_names->erase(abst.VarName());
        }

        return nameless::Abst(body);
      },
      [free_nctx, abst_var_names,
       abst_count](const named::Appl& appl) -> nameless::Term {
        nameless::Term func =
            RemoveNames(appl.Func(), free_nctx, abst_var_names, abst_count);
        nameless::Term arg =
            RemoveNames(appl.Arg(), free_nctx, abst_var_names, abst_count);
        return nameless::Appl(func, arg);
      },
      [free_nctx, abst_var_names,
       abst_count](const named::Var& var) -> nameless::Term {
        if (abst_var_names->find(var.Name()) != abst_var_names->end()) {
          return nameless::Var(abst_var_names->at(var.Name()) - abst_count);
        }
        if (!free_nctx->HasName(var.Name())) {
          free_nctx->AddName(var.Name());
        }
        return nameless::Var(free_nctx->GetIndexForName(var.Name()));
      });
}
}  // namespace

NameContext::NameContext() : idx_(0) {}

int NameContext::AddName(const std::string& name) {
  const auto name_idx = name_to_idx_.find(name);
  if (name_idx != name_to_idx_.end()) {
    // Name is already added.
    return name_idx->second;
  }

  name_to_idx_[name] = idx_;
  idx_to_name_[idx_] = name;
  return idx_++;
}

void NameContext::SetName(const std::string& name, int idx) {
  name_to_idx_[name] = idx;
  idx_to_name_[idx] = name;
}

std::string NameContext::GetNameForIndex(int idx) const {
  return idx_to_name_.at(idx);
}

int NameContext::GetIndexForName(const std::string& name) const {
  return name_to_idx_.at(name);
}

bool NameContext::HasName(const std::string& name) const {
  return name_to_idx_.find(name) != name_to_idx_.end();
}

bool NameContext::HasIndex(int idx) const {
  return idx_to_name_.find(idx) != idx_to_name_.end();
}

std::unordered_set<std::string> NameContext::Names() const {
  std::unordered_set<std::string> names;
  for (const auto& name_idx : name_to_idx_) {
    names.insert(name_idx.first);
  }
  return names;
}

named::Term AddNames(const nameless::Term& term, NameContext* free_nctx) {
  NameContext bound_nctx;
  std::unordered_set<std::string> names = free_nctx->Names();
  return AddNames(term, free_nctx, &bound_nctx, &names, 0);
}

nameless::Term RemoveNames(const named::Term& term, NameContext* free_nctx) {
  std::unordered_map<std::string, int> abst_var_names;
  return RemoveNames(term, free_nctx, &abst_var_names, 0);
}
}  // namespace transform
}  // namespace lhat
