#ifndef LHAT_NAMED_AST_H
#define LHAT_NAMED_AST_H

#include <memory>
#include <string>
#include <variant>

#include "lhat/util/overloaded.h"

namespace lhat {
namespace named {
class Term;

// Lambda term that represents function abstraction.
class Abst {
 public:
  Abst(std::string var_name, Term body);
  Abst(const Abst& other);
  Abst& operator=(const Abst& other);

  Abst(Abst&& other) = default;
  Abst& operator=(Abst&& other) = default;

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

// Lambda term that represents function application.
class Appl {
 public:
  Appl(Term func, Term arg);
  Appl(const Appl& other);
  Appl& operator=(const Appl& other);

  Appl(Appl&& other) = default;
  Appl& operator=(Appl&& other) = default;

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

// Lambda term that represents a variable.
class Var {
 public:
  Var(std::string name);

  Var(const Var& other) = default;
  Var(Var&& other) = default;
  Var& operator=(const Var& other) = default;
  Var& operator=(Var&& other) = default;

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

// Generic lambda term.
class Term {
 public:
  Term(Abst abst);
  Term(Appl appl);
  Term(Var var);

  Term(const Term& term) = default;
  Term(Term&& term) = default;
  Term& operator=(const Term& other) = default;
  Term& operator=(Term&& other) = default;

  // Returns the type of the lambda term.
  TermType Type() const;

  // Returns the concrete instance of the lambda term.
  template <class T>
  constexpr auto Get() & -> decltype(auto) {
    return std::get_if<T>(&term_);
  }

  // Returns the concrete instance of the lambda term.
  template <class T>
  constexpr auto Get() const& -> decltype(auto) {
    return std::get_if<T>(&term_);
  }

  // Passes the term to a set of type-based matchers and executes the one that
  // matches.
  template <class... M>
  constexpr auto Match(M&&... matchers) & -> decltype(auto) {
    return std::visit(util::Overload(std::forward<M>(matchers)...), term_);
  }

  // Passes the term to a set of type-based matchers and executes the one that
  // matches.
  template <class... M>
  constexpr auto Match(M&&... matchers) const& -> decltype(auto) {
    return std::visit(util::Overload(std::forward<M>(matchers)...), term_);
  }

 private:
  std::variant<Abst, Appl, Var> term_;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_AST_H
