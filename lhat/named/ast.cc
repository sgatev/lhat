#include "ast.h"

namespace lhat {
namespace named {
Term::Term(Abst abst) : term_(std::move(abst)) {}

Term::Term(Appl appl) : term_(std::move(appl)) {}

Term::Term(Var var) : term_(std::move(var)) {}

TermType Term::Type() const { return static_cast<TermType>(term_.index()); }

Abst::Abst(std::string var_name, Term body)
    : var_name_(std::move(var_name)),
      body_(std::make_unique<Term>(std::move(body))) {}

Abst::Abst(const Abst& other)
    : var_name_(other.var_name_), body_(std::make_unique<Term>(*other.body_)) {}

Abst& Abst::operator=(const Abst& other) { return *this = Abst(other); }

const std::string& Abst::VarName() const { return var_name_; }

void Abst::SetVarName(std::string var_name) { var_name_ = std::move(var_name); }

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

Var::Var(std::string name) : name_(std::move(name)) {}

const std::string& Var::Name() const { return name_; }

void Var::SetName(std::string name) { name_ = std::move(name); }
}  // namespace named
}  // namespace lhat
