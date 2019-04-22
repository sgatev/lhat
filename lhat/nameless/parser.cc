#include "lhat/nameless/parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

util::ParseResult<Term> Parser::Parse(std::istream* input) {
  return Parser(input).ParseTerm();
}

Parser::Parser(std::istream* input) : input_(input), abst_count_(0) {}

util::ParseResult<Term> Parser::ParseTerm() {
  util::DiscardWhitespace(input_);

  if (input_->eof()) {
    return util::ParseError("Failed to parse term: given expression is empty");
  }

  if (input_->peek() != '(') {
    util::ParseResult<Var> var = ParseVar();
    if (!var.Ok()) {
      return var.Error();
    }
    return Term(var.Value());
  }

  // discard '('
  input_->get();

  util::DiscardWhitespace(input_);

  if (input_->eof()) {
    return util::ParseError("Failed to parse term: ( is not closed");
  }

  if (input_->peek() == '^') {
    util::ParseResult<Abst> abst = ParseAbst();
    if (!abst.Ok()) {
      return abst.Error();
    }
    return Term(abst.Value());
  }

  util::ParseResult<Appl> appl = ParseAppl();
  if (!appl.Ok()) {
    return appl.Error();
  }
  return Term(appl.Value());
}

util::ParseResult<Abst> Parser::ParseAbst() {
  abst_count_++;

  // discard '^'
  input_->get();

  util::DiscardWhitespace(input_);

  // parse body term
  const util::ParseResult<Term> body = ParseTerm();
  if (!body.Ok()) {
    return body.Error();
  }

  util::DiscardWhitespace(input_);

  // discard ')'
  input_->get();

  abst_count_--;

  return Abst(body.Value());
}

util::ParseResult<Appl> Parser::ParseAppl() {
  util::DiscardWhitespace(input_);

  // parse func term
  const util::ParseResult<Term> func = ParseTerm();
  if (!func.Ok()) {
    return func.Error();
  }

  util::DiscardWhitespace(input_);

  // parse arg term
  const util::ParseResult<Term> arg = ParseTerm();
  if (!arg.Ok()) {
    return arg.Error();
  }

  util::DiscardWhitespace(input_);

  // discard ')'
  input_->get();

  return Appl(func.Value(), arg.Value());
}

util::ParseResult<Var> Parser::ParseVar() {
  std::string idx;
  while (!input_->eof() && input_->peek() >= 0 && !IsSpecial(input_->peek())) {
    idx.push_back(input_->get());
  }
  return Var(std::stoi(idx) - abst_count_);
}
}  // namespace nameless
}  // namespace lhat
