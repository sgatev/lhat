#include "ast.h"

namespace lhat {
namespace named {
Term::Term(TermType type) : type_(type) {}

Term::~Term() {}

TermType Term::Type() const { return type_; }

std::shared_ptr<AbstTerm> AbstTerm::Make(const std::string& var_name,
                                         std::shared_ptr<Term> body) {
  return std::make_shared<AbstTerm>(var_name, body);
}

AbstTerm::AbstTerm(const std::string& var_name, std::shared_ptr<Term> body)
    : Term(ABST_TERM), var_name(var_name), body(body) {}

std::shared_ptr<ApplTerm> ApplTerm::Make(std::shared_ptr<Term> func,
                                         std::shared_ptr<Term> arg) {
  return std::make_shared<ApplTerm>(func, arg);
}

ApplTerm::ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg)
    : Term(APPL_TERM), func(func), arg(arg) {}

std::shared_ptr<VarTerm> VarTerm::Make(const std::string& name) {
  return std::make_shared<VarTerm>(name);
}

VarTerm::VarTerm(const std::string& name) : Term(VAR_TERM), name(name) {}
}  // namespace named
}  // namespace lhat
