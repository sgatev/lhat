#include "ast.h"

namespace lhat {
namespace nameless {
Term::~Term() {}

std::shared_ptr<AbstTerm> AbstTerm::Make(std::shared_ptr<Term> body) {
  return std::make_shared<AbstTerm>(body);
}

AbstTerm::AbstTerm(std::shared_ptr<Term> body) : body(body) {}

TermType AbstTerm::Type() const { return ABST_TERM; }

std::shared_ptr<ApplTerm> ApplTerm::Make(std::shared_ptr<Term> func,
                                         std::shared_ptr<Term> arg) {
  return std::make_shared<ApplTerm>(func, arg);
}

ApplTerm::ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg)
    : func(func), arg(arg) {}

TermType ApplTerm::Type() const { return APPL_TERM; }

std::shared_ptr<VarTerm> VarTerm::VarTerm::Make(int idx) {
  return std::make_shared<VarTerm>(idx);
}

VarTerm::VarTerm(int idx) : idx(idx) {}

TermType VarTerm::Type() const { return VAR_TERM; }
}  // namespace nameless
}  // namespace lhat
