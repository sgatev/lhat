#include "ast.h"

namespace lhat {
namespace nameless {
Term::Term(TermType type) : type_(type) {}

Term::~Term() {}

TermType Term::Type() const { return type_; }

std::shared_ptr<AbstTerm> AbstTerm::Make(std::shared_ptr<Term> body) {
  return std::make_shared<AbstTerm>(body);
}

AbstTerm::AbstTerm(std::shared_ptr<Term> body) : Term(ABST_TERM), body(body) {}

std::shared_ptr<ApplTerm> ApplTerm::Make(std::shared_ptr<Term> func,
                                         std::shared_ptr<Term> arg) {
  return std::make_shared<ApplTerm>(func, arg);
}

ApplTerm::ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg)
    : Term(APPL_TERM), func(func), arg(arg) {}

std::shared_ptr<VarTerm> VarTerm::VarTerm::Make(int idx) {
  return std::make_shared<VarTerm>(idx);
}

VarTerm::VarTerm(int idx) : Term(VAR_TERM), idx(idx) {}
}  // namespace nameless
}  // namespace lhat
