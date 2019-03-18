#ifndef LHAT_PRINTER_H
#define LHAT_PRINTER_H

#include <string>

#include "ast.h"

namespace lhat {
// Parser constructs lambda expressions from ASTs.
class Printer {
public:
  // Print returns the lambda expression corresponding to term.
  static std::string Print(std::shared_ptr<Term> term);

private:
  std::string PrintTerm(std::shared_ptr<Term> term);
  std::string PrintVarTerm(std::shared_ptr<VarTerm> var);
  std::string PrintAbstTerm(std::shared_ptr<AbstTerm> abst);
  std::string PrintApplTerm(std::shared_ptr<ApplTerm> appl);

  std::string result;
};
}  // namespace lhat

#endif  // LHAT_PRINTER_H
