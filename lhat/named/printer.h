#ifndef LHAT_NAMED_PRINTER_H
#define LHAT_NAMED_PRINTER_H

#include <string>
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
  static void Print(const Term& term, std::string* out);

private:
  void Print(const Term& term);
  void Print(const Var& var);
  void Print(const Abst& abst);
  void Print(const Appl& appl);

  std::vector<absl::string_view> result_pieces_;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PRINTER_H
