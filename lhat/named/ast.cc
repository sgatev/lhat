#include "ast.h"

namespace lhat {
namespace named {
Term::~Term() {}

std::shared_ptr<AbstTerm> AbstTerm::Make(const std::string& var_name,
                                         std::shared_ptr<Term> body) {
  return std::make_shared<AbstTerm>(var_name, body);
}

AbstTerm::AbstTerm(const std::string& var_name, std::shared_ptr<Term> body)
    : var_name(var_name), body(body) {}

TermType AbstTerm::Type() const { return ABST_TERM; }

std::shared_ptr<ApplTerm> ApplTerm::Make(std::shared_ptr<Term> func,
                                         std::shared_ptr<Term> arg) {
  return std::make_shared<ApplTerm>(func, arg);
}

ApplTerm::ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg)
    : func(func), arg(arg) {}

TermType ApplTerm::Type() const { return APPL_TERM; }

std::shared_ptr<VarTerm> VarTerm::Make(const std::string& name) {
  return std::make_shared<VarTerm>(name);
}

VarTerm::VarTerm(const std::string& name) : name(name) {}

TermType VarTerm::Type() const { return VAR_TERM; }
}  // namespace named
}  // namespace lhat
