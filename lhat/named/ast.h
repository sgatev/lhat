#ifndef LHAT_NAMED_AST_H
#define LHAT_NAMED_AST_H

#include <memory>
#include <string>

namespace lhat {
namespace named {
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
  TermType Type() const;

protected:
  Term(TermType type);

private:
  TermType type_;
};

// AbstTerm is a lambda term that represents function abstraction.
class AbstTerm : public Term {
public:
  // Makes a function abstraction lambda term.
  static std::shared_ptr<AbstTerm> Make(const std::string& var_name,
                                        std::shared_ptr<Term> body);

  AbstTerm(const std::string& var_name, std::shared_ptr<Term> body);

  // Name of the function variable.
  std::string var_name;

  // Lambda term representing the body of the function.
  std::shared_ptr<Term> body;
};

// ApplTerm is a lambda term that represents function application.
class ApplTerm : public Term {
public:
  // Makes a function application lambda term.
  static std::shared_ptr<ApplTerm> Make(std::shared_ptr<Term> func,
                                        std::shared_ptr<Term> arg);

  ApplTerm(std::shared_ptr<Term> func, std::shared_ptr<Term> arg);

  // Lambda term representing the function.
  std::shared_ptr<Term> func;

  // Lambda term representing the argument.
  std::shared_ptr<Term> arg;
};

// VarTerm is a lambda term that represents a variable.
class VarTerm : public Term {
public:
  // Makes a variable lambda term.
  static std::shared_ptr<VarTerm> Make(const std::string& name);

  VarTerm(const std::string& name);

  // Name of the variable.
  std::string name;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_AST_H
