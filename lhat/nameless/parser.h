#ifndef LHAT_NAMELESS_PARSER_H
#define LHAT_NAMELESS_PARSER_H

#include "lhat/core/parse.h"
#include "lhat/io/char_reader.h"
#include "lhat/nameless/ast.h"

namespace lhat {
namespace nameless {
// Constructs ASTs from lambda terms.
class Parser {
 public:
  // Parses expr and returns the corresponding AST.
  static core::ParseResult<Term> Parse(io::CharReader* reader);

 private:
  Parser(io::CharReader* reader);

  core::ParseResult<Term> ParseTerm();
  core::ParseResult<Abst> ParseAbst();
  core::ParseResult<Appl> ParseAppl();
  core::ParseResult<Var> ParseVar();
  void ParseWhitespace();

  char Peek();
  void Next();

  io::CharReader* reader_;
  int abst_count_;
  int idx_;
};
}  // namespace nameless
}  // namespace lhat

#endif  // LHAT_NAMELESS_PARSER_H
