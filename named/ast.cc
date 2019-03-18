#include "ast.h"

namespace lhat {
namespace named {
Term::~Term() {}

AbstTerm::AbstTerm(const std::string& var_name, std::shared_ptr<Term> body)
    : var_name(var_name), body(body) {}

TermType AbstTerm::Type() const { return ABST_TERM; }

ApplTerm::ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg)
    : func(func), arg(arg) {}

TermType ApplTerm::Type() const { return APPL_TERM; }

VarTerm::VarTerm(const std::string& name) : name(name) {}

TermType VarTerm::Type() const { return VAR_TERM; }
}  // namespace named
}  // namespace lhat
