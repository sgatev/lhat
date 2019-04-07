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
  static Term Parse(const std::string& expr);

private:
  Parser(const std::string& expr);

  Term ParseTerm();
  Abst ParseAbst();
  Appl ParseAppl();
  Var ParseVar();

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
