#include "printer.h"

namespace lhat {
namespace named {
std::string Printer::Print(std::shared_ptr<Term> term) {
  if (term == nullptr) {
    return "";
  }
  return Printer().PrintTerm(term);
}

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
  return var->name;
}

std::string Printer::PrintAbstTerm(std::shared_ptr<AbstTerm> abst) {
  return "(^ " + abst->var_name + " " + PrintTerm(abst->body) + ")";
}

std::string Printer::PrintApplTerm(std::shared_ptr<ApplTerm> appl) {
  return "(" + PrintTerm(appl->func) + " " + PrintTerm(appl->arg) + ")";
}
}  // namespace named
}  // namespace lhat
