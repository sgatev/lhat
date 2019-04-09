#ifndef LHAT_NAMED_AST_H
#define LHAT_NAMED_AST_H

#include <memory>
#include <string>
#include <variant>

#include "lhat/core/overloaded.h"

namespace lhat {
namespace named {
class Term;

// Abst is a lambda term that represents function abstraction.
class Abst {
public:
  Abst(std::string var_name, Term body);
  Abst(const Abst& other);
  Abst(Abst&& other) noexcept = default;

  Abst& operator=(const Abst& other);
  Abst& operator=(Abst&& other) noexcept = default;

  // Returns the variable name of the function.
  const std::string& VarName() const;

  // Sets the variable name of the function.
  void SetVarName(std::string var_name);

  // Returns the lambda term representing the body of the function.
  Term* MutableBody();

  // Returns the lambda term representing the body of the function.
  const Term& Body() const;

private:
  std::string var_name_;
  std::unique_ptr<Term> body_;
};

// Appl is a lambda term that represents function application.
class Appl {
public:
  Appl(Term func, Term arg);
  Appl(const Appl& other);
  Appl(Appl&& other) noexcept = default;

  Appl& operator=(const Appl& other);
  Appl& operator=(Appl&& other) noexcept = default;

  // Returns the lambda term representing the function.
  Term* MutableFunc();

  // Returns the lambda term representing the function.
  const Term& Func() const;

  // Returns the lambda term representing the argument.
  Term* MutableArg();

  // Returns the lambda term representing the argument.
  const Term& Arg() const;

private:
  std::unique_ptr<Term> func_;
  std::unique_ptr<Term> arg_;
};

// Var is a lambda term that represents a variable.
class Var {
public:
  Var(std::string name);
  Var(const Var& other) = default;
  Var(Var&& other) noexcept = default;

  Var& operator=(const Var& other) = default;
  Var& operator=(Var&& other) noexcept = default;

  // Returns the name of the variable.
  const std::string& Name() const;

  // Sets the name of the variable.
  void SetName(std::string name);

private:
  std::string name_;
};

// Types of nameless lambda terms.
enum TermType {
  ABST,
  APPL,
  VAR,
};

// Term is a generic lambda term.
class Term {
public:
  Term(Abst abst);
  Term(Appl appl);
  Term(Var var);
  Term(const Term& term) = default;
  Term(Term&& term) noexcept = default;

  Term& operator=(const Term& other) = default;
  Term& operator=(Term&& other) noexcept = default;

  // Returns the type of the lambda term.
  TermType Type() const;

  template <class T>
  constexpr auto Get() & -> decltype(auto) {
    return std::get_if<T>(&term_);
  }

  template <class T>
  constexpr auto Get() const& -> decltype(auto) {
    return std::get_if<T>(&term_);
  }

  template <class... M>
  constexpr auto Match(M&&... matchers) & -> decltype(auto) {
    return std::visit(core::Overload(std::forward<M>(matchers)...), term_);
  }

  template <class... M>
  constexpr auto Match(M&&... matchers) const& -> decltype(auto) {
    return std::visit(core::Overload(std::forward<M>(matchers)...), term_);
  }

private:
  std::variant<Abst, Appl, Var> term_;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_AST_H
