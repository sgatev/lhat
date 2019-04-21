#include "lhat/nameless/parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

core::ParseResult<Term> Parser::Parse(std::istream* input) {
  return Parser(input).ParseTerm();
}

Parser::Parser(std::istream* input) : input_(input), abst_count_(0) {}

core::ParseResult<Term> Parser::ParseTerm() {
  core::DiscardWhitespace(input_);

  if (input_->eof()) {
    return core::ParseError("Failed to parse term: given expression is empty");
  }

  if (input_->peek() != '(') {
    core::ParseResult<Var> var = ParseVar();
    if (!var.Ok()) {
      return var.Error();
    }
    return Term(var.Value());
  }

  // discard '('
  input_->get();

  core::DiscardWhitespace(input_);

  if (input_->eof()) {
    return core::ParseError("Failed to parse term: ( is not closed");
  }

  if (input_->peek() == '^') {
    core::ParseResult<Abst> abst = ParseAbst();
    if (!abst.Ok()) {
      return abst.Error();
    }
    return Term(abst.Value());
  }

  core::ParseResult<Appl> appl = ParseAppl();
  if (!appl.Ok()) {
    return appl.Error();
  }
  return Term(appl.Value());
}

core::ParseResult<Abst> Parser::ParseAbst() {
  abst_count_++;

  // discard '^'
  input_->get();

  core::DiscardWhitespace(input_);

  // parse body term
  const core::ParseResult<Term> body = ParseTerm();
  if (!body.Ok()) {
    return body.Error();
  }

  core::DiscardWhitespace(input_);

  // discard ')'
  input_->get();

  abst_count_--;

  return Abst(body.Value());
}

core::ParseResult<Appl> Parser::ParseAppl() {
  core::DiscardWhitespace(input_);

  // parse func term
  const core::ParseResult<Term> func = ParseTerm();
  if (!func.Ok()) {
    return func.Error();
  }

  core::DiscardWhitespace(input_);

  // parse arg term
  const core::ParseResult<Term> arg = ParseTerm();
  if (!arg.Ok()) {
    return arg.Error();
  }

  core::DiscardWhitespace(input_);

  // discard ')'
  input_->get();

  return Appl(func.Value(), arg.Value());
}

core::ParseResult<Var> Parser::ParseVar() {
  std::string idx;
  while (!input_->eof() && input_->peek() >= 0 && !IsSpecial(input_->peek())) {
    idx.push_back(input_->get());
  }
  return Var(std::stoi(idx) - abst_count_);
}
}  // namespace nameless
}  // namespace lhat
