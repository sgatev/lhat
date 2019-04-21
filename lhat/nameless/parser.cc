#include "lhat/nameless/parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
}  // namespace

core::ParseResult<Term> Parser::Parse(std::istream* input) {
  return Parser(input).ParseTerm();
}

Parser::Parser(std::istream* input) : input_(input), abst_count_(0) {}

core::ParseResult<Term> Parser::ParseTerm() {
  ParseWhitespace();

  if (input_->eof()) {
    return core::ParseResult<Term>(
        core::ParseError("Failed to parse term: given expression is empty"));
  }

  if (input_->peek() != '(') {
    core::ParseResult<Var> var = ParseVar();
    if (!var.Ok()) {
      return core::ParseResult<Term>(var.Error());
    }
    return core::ParseResult<Term>(Term(var.Value()));
  }

  // parse '('
  input_->get();

  ParseWhitespace();

  if (input_->eof()) {
    return core::ParseResult<Term>(
        core::ParseError("Failed to parse term: ( is not closed"));
  }

  if (input_->peek() == '^') {
    core::ParseResult<Abst> abst = ParseAbst();
    if (!abst.Ok()) {
      return core::ParseResult<Term>(abst.Error());
    }
    return core::ParseResult<Term>(Term(abst.Value()));
  }

  core::ParseResult<Appl> appl = ParseAppl();
  if (!appl.Ok()) {
    return core::ParseResult<Term>(appl.Error());
  }
  return core::ParseResult<Term>(Term(appl.Value()));
}

core::ParseResult<Abst> Parser::ParseAbst() {
  abst_count_++;

  // parse '^'
  input_->get();

  ParseWhitespace();

  // parse body term
  const core::ParseResult<Term> body = ParseTerm();
  if (!body.Ok()) {
    return core::ParseResult<Abst>(body.Error());
  }

  ParseWhitespace();

  // parse ')'
  input_->get();

  abst_count_--;

  return core::ParseResult<Abst>(Abst(body.Value()));
}

core::ParseResult<Appl> Parser::ParseAppl() {
  ParseWhitespace();

  // parse func term
  const core::ParseResult<Term> func = ParseTerm();
  if (!func.Ok()) {
    return core::ParseResult<Appl>(func.Error());
  }

  ParseWhitespace();

  // parse arg term
  const core::ParseResult<Term> arg = ParseTerm();
  if (!arg.Ok()) {
    return core::ParseResult<Appl>(arg.Error());
  }

  ParseWhitespace();

  // consume ')'
  input_->get();

  return core::ParseResult<Appl>(Appl(func.Value(), arg.Value()));
}

core::ParseResult<Var> Parser::ParseVar() {
  std::string idx;
  while (!input_->eof() && input_->peek() >= 0 && !IsSpecial(input_->peek())) {
    idx.push_back(input_->get());
  }
  return core::ParseResult<Var>(Var(std::stoi(idx) - abst_count_));
}

void Parser::ParseWhitespace() {
  while (!input_->eof() && IsWhitespace(input_->peek())) {
    input_->get();
  }
}
}  // namespace nameless
}  // namespace lhat
