#include "printer.h"

namespace lhat {
namespace named {
void Printer::Print(const Term& term) {
  term.Match([this](const auto& value) { this->Print(value); });
}

void Printer::Print(const Var& var) { result_pieces_.push_back(var.Name()); }

void Printer::Print(const Abst& abst) {
  result_pieces_.push_back("(^ ");
  result_pieces_.push_back(abst.VarName());
  result_pieces_.push_back(" ");
  Print(abst.Body());
  result_pieces_.push_back(")");
}

void Printer::Print(const Appl& appl) {
  result_pieces_.push_back("(");
  Print(appl.Func());
  result_pieces_.push_back(" ");
  Print(appl.Arg());
  result_pieces_.push_back(")");
}
}  // namespace named
}  // namespace lhat
