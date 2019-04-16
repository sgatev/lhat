#ifndef LHAT_NAMELESS_PARSER_H
#define LHAT_NAMELESS_PARSER_H

#include <string>

#include "lhat/core/parse.h"
#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Constructs ASTs from lambda terms.
class Parser {
 public:
  // Parses expr and returns the corresponding AST.
  static core::ParseResult<Term> Parse(const std::string_view expr);

 private:
  Parser(const std::string_view expr);

  core::ParseResult<Term> ParseTerm();
  core::ParseResult<Abst> ParseAbst();
  core::ParseResult<Appl> ParseAppl();
  core::ParseResult<Var> ParseVar();
  void ParseWhitespace();

  char Peek();
  void Next();

  const std::string_view expr_;
  int abst_count_;
  int idx_;
  bool done_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PARSER_H
