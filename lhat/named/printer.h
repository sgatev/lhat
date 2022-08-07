#ifndef LHAT_NAMED_PRINTER_H
#define LHAT_NAMED_PRINTER_H

#include <string>
#include <vector>

#include "lhat/named/ast.h"

namespace lhat::named {

// Constructs lambda expressions from ASTs.
class Printer {
 public:
  // Outputs the lambda expression corresponding to term in out.
  static void Print(const Term& term, std::string* out);

 private:
  void Print(const Term& term);
  void Print(const Var& var);
  void Print(const Abst& abst);
  void Print(const Appl& appl);

  std::vector<std::string_view> result_pieces_;
};

}  // namespace lhat::named

#endif  // LHAT_NAMED_PRINTER_H
