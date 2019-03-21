#include "sub.h"

#include <unordered_set>

#include "vars.h"

namespace lhat {
namespace named {
namespace {
std::shared_ptr<Term> RenameBoundVarInTerm(const std::shared_ptr<Term> term,
                                           const std::string& from,
                                           const std::string& to);

std::shared_ptr<AbstTerm> RenameBoundVarInAbstTerm(
    const std::shared_ptr<AbstTerm> abst, const std::string& from,
    const std::string& to) {
  if (abst->var_name == from) {
    return AbstTerm::Make(to, Sub(abst->body, from, VarTerm::Make(to)));
  } else {
    return AbstTerm::Make(abst->var_name,
                          RenameBoundVarInTerm(abst->body, from, to));
  }
}

std::shared_ptr<ApplTerm> RenameBoundVarInApplTerm(
    const std::shared_ptr<ApplTerm> appl, const std::string& from,
    const std::string& to) {
  return ApplTerm::Make(RenameBoundVarInTerm(appl->func, from, to),
                        RenameBoundVarInTerm(appl->arg, from, to));
}

std::shared_ptr<VarTerm> RenameBoundVarInVarTerm(
    const std::shared_ptr<VarTerm> var, const std::string& from,
    const std::string& to) {
  return VarTerm::Make(var->name);
}

std::shared_ptr<Term> RenameBoundVarInTerm(const std::shared_ptr<Term> term,
                                           const std::string& from,
                                           const std::string& to) {
  switch (term->Type()) {
    case ABST_TERM:
      return RenameBoundVarInAbstTerm(std::static_pointer_cast<AbstTerm>(term),
                                      from, to);
      break;
    case APPL_TERM:
      return RenameBoundVarInApplTerm(std::static_pointer_cast<ApplTerm>(term),
                                      from, to);
      break;
    case VAR_TERM:
      return RenameBoundVarInVarTerm(std::static_pointer_cast<VarTerm>(term),
                                     from, to);
  }
}

std::shared_ptr<Term> SubAbstTerm(const std::shared_ptr<AbstTerm> target,
                                  const std::string& var_name,
                                  const std::shared_ptr<Term> term) {
  if (target->var_name == var_name) {
    return target;
  }
  return AbstTerm::Make(target->var_name, Sub(target->body, var_name, term));
}

std::shared_ptr<Term> SubApplTerm(const std::shared_ptr<ApplTerm> target,
                                  const std::string& var_name,
                                  const std::shared_ptr<Term> term) {
  return ApplTerm::Make(Sub(target->func, var_name, term),
                        Sub(target->arg, var_name, term));
}

std::shared_ptr<Term> SubVarTerm(const std::shared_ptr<VarTerm> target,
                                 const std::string& var_name,
                                 const std::shared_ptr<Term> term) {
  if (target->name == var_name) {
    return term;
  } else {
    return target;
  }
}
}  // namespace

std::shared_ptr<Term> SafeSub(const std::shared_ptr<Term> target,
                              const std::string& var_name,
                              const std::shared_ptr<Term> term) {
  if (target == nullptr) {
    return nullptr;
  }
  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(target, &var_names);
  InsertFreeVarNames(term, &var_names);
  std::string new_var_name = NewVarName(var_names);
  return Sub(target, var_name, term);
}

std::shared_ptr<Term> Sub(const std::shared_ptr<Term> target,
                          const std::string& var_name,
                          const std::shared_ptr<Term> term) {
  if (target == nullptr) {
    return nullptr;
  }
  switch (target->Type()) {
    case ABST_TERM:
      return SubAbstTerm(std::static_pointer_cast<AbstTerm>(target), var_name,
                         term);
    case APPL_TERM:
      return SubApplTerm(std::static_pointer_cast<ApplTerm>(target), var_name,
                         term);
    case VAR_TERM:
      return SubVarTerm(std::static_pointer_cast<VarTerm>(target), var_name,
                        term);
  }
}
}  // namespace named
}  // namespace lhat
