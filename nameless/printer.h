#ifndef LHAT_NAMELESS_PRINTER_H
#define LHAT_NAMELESS_PRINTER_H

#include <string>

#include "ast.h"

namespace lhat {
namespace nameless {
// Parser constructs lambda expressions from ASTs.
class Printer {
public:
  // Print returns the lambda expression corresponding to term.
  static std::string Print(std::shared_ptr<Term> term);

private:
  Printer();

  std::string PrintTerm(std::shared_ptr<Term> term);
  std::string PrintVarTerm(std::shared_ptr<VarTerm> var);
  std::string PrintAbstTerm(std::shared_ptr<AbstTerm> abst);
  std::string PrintApplTerm(std::shared_ptr<ApplTerm> appl);

  int abst_count_;
  std::string result_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PRINTER_H
