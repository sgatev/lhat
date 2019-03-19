#ifndef LHAT_NAMED_PRINTER_H
#define LHAT_NAMED_PRINTER_H

#include <vector>

#include "ast.h"

#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"
#include "absl/strings/string_view.h"

namespace lhat {
namespace named {
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
  void PrintTerm(const std::shared_ptr<Term> term);
  void PrintVarTerm(const std::shared_ptr<VarTerm> var);
  void PrintAbstTerm(const std::shared_ptr<AbstTerm> abst);
  void PrintApplTerm(const std::shared_ptr<ApplTerm> appl);

  std::vector<absl::string_view> result_pieces_;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PRINTER_H
