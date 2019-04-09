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
  static void Print(const Term& term, std::string* out);

private:
  Printer();

  void Print(const Term& term);
  void Print(const Abst& abst);
  void Print(const Appl& appl);
  void Print(const Var& var);

  int abst_count_;
  std::vector<std::string> result_pieces_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PRINTER_H
