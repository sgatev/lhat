#ifndef LHAT_NAMELESS_PARSER_H
#define LHAT_NAMELESS_PARSER_H

#include <istream>

#include "lhat/nameless/ast.h"
#include "lhat/util/parse.h"

namespace lhat {
namespace nameless {
// Constructs ASTs from lambda terms.
class Parser {
 public:
  // Parses expr and returns the corresponding AST.
  static util::ParseResult<Term> Parse(std::istream* input);

 private:
  Parser(std::istream* input);

  util::ParseResult<Term> ParseTerm();
  util::ParseResult<Abst> ParseAbst();
  util::ParseResult<Appl> ParseAppl();
  util::ParseResult<Var> ParseVar();

  std::istream* input_;
  int abst_count_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PARSER_H
