#ifndef LHAT_NAMELESS_PRINTER_H
#define LHAT_NAMELESS_PRINTER_H

#include <string>
#include <vector>

#include "ast.h"

#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"

namespace lhat {
namespace nameless {
// Parser constructs lambda expressions from ASTs.
class Printer {
public:
  // Print returns the lambda expression corresponding to term.
  template <class OutType>
  static void Print(const std::shared_ptr<Term> term, OutType out) {
    Printer printer;
    printer.PrintTerm(term);
    absl::StrAppend(out, absl::StrJoin(printer.result_pieces_, ""));
  }

private:
  Printer();

  void PrintTerm(const std::shared_ptr<Term> term);
  void PrintVarTerm(const std::shared_ptr<VarTerm> var);
  void PrintAbstTerm(const std::shared_ptr<AbstTerm> abst);
  void PrintApplTerm(const std::shared_ptr<ApplTerm> appl);

  int abst_count_;
  std::vector<std::string> result_pieces_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PRINTER_H
