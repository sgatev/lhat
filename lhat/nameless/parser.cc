#include "lhat/nameless/parser.h"

#include "lhat/util/parse.h"

namespace lhat::nameless {
namespace {

bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

}  // namespace

util::ErrorOr<Term> Parser::Parse(std::istream* input) {
  return Parser(input).ParseTerm();
}

Parser::Parser(std::istream* input) : input_(input), abst_count_(0) {}

util::ErrorOr<Term> Parser::ParseTerm() {
  util::DiscardWhitespace(input_);

  if (input_->eof()) {
    return util::Error("Failed to parse term: given expression is empty");
  }

  if (input_->peek() != '(') {
    util::ErrorOr<Var> var = ParseVar();
    RETURN_IF_ERROR(var);
    return Term(var.Value());
  }

  // discard '('
  input_->get();

  util::DiscardWhitespace(input_);

  if (input_->eof()) {
    return util::Error("Failed to parse term: ( is not closed");
  }

  if (input_->peek() == '^') {
    util::ErrorOr<Abst> abst = ParseAbst();
    RETURN_IF_ERROR(abst);
    return Term(abst.Value());
  }

  util::ErrorOr<Appl> appl = ParseAppl();
  RETURN_IF_ERROR(appl);
  return Term(appl.Value());
}

util::ErrorOr<Abst> Parser::ParseAbst() {
  abst_count_++;

  // discard '^'
  input_->get();

  util::DiscardWhitespace(input_);

  // parse body term
  const util::ErrorOr<Term> body = ParseTerm();
  RETURN_IF_ERROR(body);

  util::DiscardWhitespace(input_);

  // discard ')'
  input_->get();

  abst_count_--;

  return Abst(body.Value());
}

util::ErrorOr<Appl> Parser::ParseAppl() {
  util::DiscardWhitespace(input_);

  // parse func term
  const util::ErrorOr<Term> func = ParseTerm();
  RETURN_IF_ERROR(func);

  util::DiscardWhitespace(input_);

  // parse arg term
  const util::ErrorOr<Term> arg = ParseTerm();
  RETURN_IF_ERROR(arg);

  util::DiscardWhitespace(input_);

  // discard ')'
  input_->get();

  return Appl(func.Value(), arg.Value());
}

util::ErrorOr<Var> Parser::ParseVar() {
  std::string var_idx;
  while (!input_->eof() && input_->peek() >= 0 && !IsSpecial(input_->peek())) {
    var_idx.push_back(input_->get());
  }
  const int idx = std::stoi(var_idx);

  if (idx < abst_count_) {
    return Var(-idx - 1);
  }
  return Var(idx - abst_count_);
}

}  // namespace lhat::nameless
