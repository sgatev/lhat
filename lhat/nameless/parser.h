#ifndef LHAT_NAMELESS_PARSER_H
#define LHAT_NAMELESS_PARSER_H

#include <istream>

#include "lhat/core/parse.h"
#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Constructs ASTs from lambda terms.
class Parser {
 public:
  // Parses expr and returns the corresponding AST.
  static core::ParseResult<Term> Parse(std::istream* input);

 private:
  Parser(std::istream* input);

  core::ParseResult<Term> ParseTerm();
  core::ParseResult<Abst> ParseAbst();
  core::ParseResult<Appl> ParseAppl();
  core::ParseResult<Var> ParseVar();
  void ParseWhitespace();

  std::istream* input_;
  int abst_count_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PARSER_H
