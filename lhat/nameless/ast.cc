#include "lhat/nameless/ast.h"

namespace lhat::nameless {

Term::Term(Abst abst) : term_(std::move(abst)) {}

Term::Term(Appl appl) : term_(std::move(appl)) {}

Term::Term(Var var) : term_(std::move(var)) {}

TermType Term::Type() const { return static_cast<TermType>(term_.index()); }

Abst::Abst(Term body) : body_(std::make_unique<Term>(std::move(body))) {}

Abst::Abst(const Abst& other) : body_(std::make_unique<Term>(*other.body_)) {}

Abst& Abst::operator=(const Abst& other) { return *this = Abst(other); }

Term* Abst::MutableBody() { return body_.get(); }

const Term& Abst::Body() const { return *body_; }

Appl::Appl(Term func, Term arg)
    : func_(std::make_unique<Term>(std::move(func))),
      arg_(std::make_unique<Term>(std::move(arg))) {}

Appl::Appl(const Appl& other)
    : func_(std::make_unique<Term>(*other.func_)),
      arg_(std::make_unique<Term>(*other.arg_)) {}

Appl& Appl::operator=(const Appl& other) { return *this = Appl(other); }

Term* Appl::MutableFunc() { return func_.get(); }

const Term& Appl::Func() const { return *func_; }

Term* Appl::MutableArg() { return arg_.get(); }

const Term& Appl::Arg() const { return *arg_; }

Var::Var(int idx) : idx_(idx) {}

int Var::Index() const { return idx_; }

void Var::SetIndex(int idx) { idx_ = idx; }

}  // namespace lhat::nameless
