#include "printer.h"

namespace lhat {
namespace nameless {
std::string Printer::Print(std::shared_ptr<Term> term) {
  if (term == nullptr) {
    return "";
  }
  return Printer().PrintTerm(term);
}

Printer::Printer() : abst_count_(0) {}

std::string Printer::PrintTerm(std::shared_ptr<Term> term) {
  switch (term->Type()) {
    case VAR_TERM:
      return PrintVarTerm(std::static_pointer_cast<VarTerm>(term));
    case ABST_TERM:
      return PrintAbstTerm(std::static_pointer_cast<AbstTerm>(term));
    case APPL_TERM:
      return PrintApplTerm(std::static_pointer_cast<ApplTerm>(term));
  }
  return "";
}

std::string Printer::PrintVarTerm(std::shared_ptr<VarTerm> var) {
  return std::to_string(var->idx + abst_count_);
}

std::string Printer::PrintAbstTerm(std::shared_ptr<AbstTerm> abst) {
  abst_count_++;
  const std::string result = "(^ " + PrintTerm(abst->body) + ")";
  abst_count_--;
  return result;
}

std::string Printer::PrintApplTerm(std::shared_ptr<ApplTerm> appl) {
  return "(" + PrintTerm(appl->func) + " " + PrintTerm(appl->arg) + ")";
}
}  // namespace nameless
}  // namespace lhat
