#include "names.h"

#include <memory>
#include <unordered_map>

namespace lhat {
namespace {
std::shared_ptr<nameless::Term> StripNamesFromTerm(
    const std::shared_ptr<named::Term> term, NameContext* nctx,
    std::unordered_map<std::string, int>* abst_var_names, int abst_count);

std::shared_ptr<nameless::AbstTerm> StripNamesFromAbstTerm(
    const std::shared_ptr<named::AbstTerm> abst, NameContext* nctx,
    std::unordered_map<std::string, int>* abst_var_names, int abst_count) {
  int old_abst_var_name_idx = -1;
  if (abst_var_names->find(abst->var_name) != abst_var_names->end()) {
    old_abst_var_name_idx = abst_var_names->at(abst->var_name);
  }
  abst_var_names->insert({abst->var_name, abst_count});

  const std::shared_ptr<nameless::Term> body =
      StripNamesFromTerm(abst->body, nctx, abst_var_names, abst_count + 1);

  if (old_abst_var_name_idx >= 0) {
    abst_var_names->at(abst->var_name) = old_abst_var_name_idx;
  } else {
    abst_var_names->erase(abst->var_name);
  }

  return nameless::AbstTerm::Make(body);
}

std::shared_ptr<nameless::ApplTerm> StripNamesFromApplTerm(
    const std::shared_ptr<named::ApplTerm> appl, NameContext* nctx,
    std::unordered_map<std::string, int>* abst_var_names, int abst_count) {
  return nameless::ApplTerm::Make(
      StripNamesFromTerm(appl->func, nctx, abst_var_names, abst_count),
      StripNamesFromTerm(appl->arg, nctx, abst_var_names, abst_count));
}

std::shared_ptr<nameless::VarTerm> StripNamesFromVarTerm(
    const std::shared_ptr<named::VarTerm> var, NameContext* nctx,
    std::unordered_map<std::string, int>* abst_var_names, int abst_count) {
  if (abst_var_names->find(var->name) != abst_var_names->end()) {
    return nameless::VarTerm::Make(-abst_var_names->at(var->name) - 1);
  }
  if (!nctx->HasName(var->name)) {
    nctx->AddName(var->name);
  }
  return nameless::VarTerm::Make(nctx->GetIndexForName(var->name));
}

std::shared_ptr<nameless::Term> StripNamesFromTerm(
    const std::shared_ptr<named::Term> term, NameContext* nctx,
    std::unordered_map<std::string, int>* abst_var_names, int abst_count) {
  if (term == nullptr) {
    return nullptr;
  }
  switch (term->Type()) {
    case named::ABST_TERM:
      return StripNamesFromAbstTerm(
          std::static_pointer_cast<named::AbstTerm>(term), nctx, abst_var_names,
          abst_count);
    case named::APPL_TERM:
      return StripNamesFromApplTerm(
          std::static_pointer_cast<named::ApplTerm>(term), nctx, abst_var_names,
          abst_count);
    case named::VAR_TERM:
      return StripNamesFromVarTerm(
          std::static_pointer_cast<named::VarTerm>(term), nctx, abst_var_names,
          abst_count);
  }
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

std::shared_ptr<nameless::Term> StripNames(
    const std::shared_ptr<named::Term> term) {
  NameContext nctx;
  std::unordered_map<std::string, int> abst_var_names;
  return StripNamesFromTerm(term, &nctx, &abst_var_names, 0);
}
}  // namespace lhat
