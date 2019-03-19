#include "printer.h"

#include <memory>

namespace lhat {
namespace nameless {
Printer::Printer() : abst_count_(0) {}

void Printer::PrintTerm(const std::shared_ptr<Term> term) {
  if (term == nullptr) {
    return;
  }
  switch (term->Type()) {
    case VAR_TERM:
      PrintVarTerm(std::static_pointer_cast<VarTerm>(term));
      break;
    case ABST_TERM:
      PrintAbstTerm(std::static_pointer_cast<AbstTerm>(term));
      break;
    case APPL_TERM:
      PrintApplTerm(std::static_pointer_cast<ApplTerm>(term));
      break;
  }
}

void Printer::PrintVarTerm(const std::shared_ptr<VarTerm> var) {
  result_pieces_.push_back(std::to_string(var->idx + abst_count_));
}

void Printer::PrintAbstTerm(const std::shared_ptr<AbstTerm> abst) {
  abst_count_++;
  result_pieces_.push_back("(^ ");
  PrintTerm(abst->body);
  result_pieces_.push_back(")");
  abst_count_--;
}

void Printer::PrintApplTerm(const std::shared_ptr<ApplTerm> appl) {
  result_pieces_.push_back("(");
  PrintTerm(appl->func);
  result_pieces_.push_back(" ");
  PrintTerm(appl->arg);
  result_pieces_.push_back(")");
}
}  // namespace nameless
}  // namespace lhat
