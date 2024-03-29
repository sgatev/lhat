#include "lhat/nameless/printer.h"

#include "absl/strings/str_cat.h"
#include "absl/strings/str_join.h"

namespace lhat::nameless {

void Printer::Print(const Term& term, std::string* out) {
  Printer printer;
  printer.Print(term);
  absl::StrAppend(out, absl::StrJoin(printer.result_pieces_, ""));
}

Printer::Printer() : abst_count_(0) {}

void Printer::Print(const Term& term) {
  term.Match([this](const auto& value) { this->Print(value); });
}

void Printer::Print(const Var& var) {
  if (var.Index() >= 0) {
    result_pieces_.push_back(std::to_string(var.Index() + abst_count_));
  } else {
    result_pieces_.push_back(std::to_string(-var.Index() - 1));
  }
}

void Printer::Print(const Abst& abst) {
  abst_count_++;
  result_pieces_.push_back("(^ ");
  Print(abst.Body());
  result_pieces_.push_back(")");
  abst_count_--;
}

void Printer::Print(const Appl& appl) {
  result_pieces_.push_back("(");
  Print(appl.Func());
  result_pieces_.push_back(" ");
  Print(appl.Arg());
  result_pieces_.push_back(")");
}

}  // namespace lhat::nameless
