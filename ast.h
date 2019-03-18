#ifndef LHAT_AST_H
#define LHAT_AST_H

#include <string>

namespace lhat {
// TermType is an enumeration of types of lambda terms.
enum TermType {
  // ABST_TERM is a function abstraction lambda term.
  ABST_TERM,

  // APPL_TERM is a function application lambda term.
  APPL_TERM,

  // VAR_TERM is a variable lambda term.
  VAR_TERM,
};

// Term is a generic lambda term.
class Term {
public:
  virtual ~Term();

  // Type returns the type of the term.
  virtual TermType Type() const = 0;
};

// AbstTerm is a lambda term that represents function abstraction.
class AbstTerm : public Term {
public:
  AbstTerm(const std::string& var_name, std::shared_ptr<Term> body);

  TermType Type() const override;

  // Name of the function variable.
  std::string var_name;

  // Lambda term representing the body of the function.
  std::shared_ptr<Term> body;
};

// ApplTerm is a lambda term that represents function application.
class ApplTerm : public Term {
public:
  ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg);

  TermType Type() const override;

  // Lambda term representing the function.
  std::shared_ptr<Term> func;

  // Lambda term representing the argument.
  std::shared_ptr<Term> arg;
};

// VarTerm is a lambda term that represents a variable.
class VarTerm : public Term {
public:
  VarTerm(const std::string& name);

  TermType Type() const override;

  // Name of the variable.
  std::string name;
};

}  // namespace lhat

#endif  // LHAT_AST_H
