#ifndef LHAT_NAMED_PRINTER_H
#define LHAT_NAMED_PRINTER_H

#include <string>
#include <vector>

#include "ast.h"

namespace lhat {
namespace named {
// Parser constructs lambda expressions from ASTs.
class Printer {
public:
  // Print returns the lambda expression corresponding to term.
  static std::string Print(const std::shared_ptr<Term> term);

private:
  void PrintTerm(const std::shared_ptr<Term> term);
  void PrintVarTerm(const std::shared_ptr<VarTerm> var);
  void PrintAbstTerm(const std::shared_ptr<AbstTerm> abst);
  void PrintApplTerm(const std::shared_ptr<ApplTerm> appl);

  std::vector<std::string> result_pieces_;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PRINTER_H
