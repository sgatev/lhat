#ifndef LHAT_NAMELESS_PARSER_H
#define LHAT_NAMELESS_PARSER_H

#include <string>

#include "ast.h"

namespace lhat {
namespace nameless {
// Parser constructs ASTs from lambda terms.
class Parser {
public:
  // Parse parses expr and returns the corresponding AST.
  static std::shared_ptr<Term> Parse(const std::string& expr);

private:
  Parser(const std::string& expr);

  std::shared_ptr<Term> ParseTerm();
  std::shared_ptr<AbstTerm> ParseAbstTerm();
  std::shared_ptr<ApplTerm> ParseApplTerm();
  std::shared_ptr<VarTerm> ParseVarTerm();

  char Peek();
  void Next();

  const std::string& expr_;
  int abst_count_;
  int idx_;
  bool done_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PARSER_H
