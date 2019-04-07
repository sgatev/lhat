#ifndef LHAT_NAMED_PARSER_H
#define LHAT_NAMED_PARSER_H

#include <string>

#include "ast.h"

namespace lhat {
namespace named {
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
  std::string ParseName();

  char Peek();
  void Next();

  const std::string& expr_;
  int idx_;
  bool done_;
};
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_PARSER_H
