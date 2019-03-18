#include "ast.h"

namespace lhat {
namespace nameless {
Term::~Term() {}

AbstTerm::AbstTerm(std::shared_ptr<Term> body) : body(body) {}

TermType AbstTerm::Type() const { return ABST_TERM; }

ApplTerm::ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg)
    : func(func), arg(arg) {}

TermType ApplTerm::Type() const { return APPL_TERM; }

VarTerm::VarTerm(int idx) : idx(idx) {}

TermType VarTerm::Type() const { return VAR_TERM; }
}  // namespace nameless
}  // namespace lhat
