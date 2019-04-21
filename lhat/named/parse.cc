#include "lhat/named/parse.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

core::ParseResult<std::string> ParseName(std::istream* input) {
  std::string name;
  while (!input->eof() && input->peek() >= 0 && !IsSpecial(input->peek())) {
    name.push_back(input->get());
  }
  return name;
}

core::ParseResult<Var> ParseVar(std::istream* input) {
  core::ParseResult<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return var_name.Error();
  }
  return Var(var_name.Value());
}

core::ParseResult<Abst> ParseAbst(std::istream* input) {
  // discard '^'
  input->get();

  core::DiscardWhitespace(input);

  // parse var name
  core::ParseResult<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return var_name.Error();
  }

  core::DiscardWhitespace(input);

  // parse body term
  core::ParseResult<Term> term = Parse(input);
  if (!term.Ok()) {
    return term.Error();
  }

  core::DiscardWhitespace(input);

  // discard ')'
  input->get();

  return Abst(var_name.Value(), term.Value());
}

core::ParseResult<Appl> ParseAppl(std::istream* input) {
  core::DiscardWhitespace(input);

  // parse func term
  core::ParseResult<Term> left = Parse(input);
  if (!left.Ok()) {
    return left.Error();
  }

  core::DiscardWhitespace(input);

  // parse arg term
  core::ParseResult<Term> right = Parse(input);
  if (!right.Ok()) {
    return right.Error();
  }

  core::DiscardWhitespace(input);

  // discard ')'
  input->get();

  return Appl(left.Value(), right.Value());
}
}  // namespace

core::ParseResult<Term> Parse(std::istream* input) {
  core::DiscardWhitespace(input);

  if (input->eof()) {
    return core::ParseError("Failed to parse term: given expression is empty");
  }
  if (input->peek() != '(') {
    core::ParseResult<Var> var = ParseVar(input);
    if (!var.Ok()) {
      return var.Error();
    }
    return Term(var.Value());
  }

  // discard '('
  input->get();

  core::DiscardWhitespace(input);

  if (input->eof()) {
    return core::ParseError("Failed to parse term: ( is not closed");
  }

  if (input->peek() == '^') {
    core::ParseResult<Abst> abst = ParseAbst(input);
    if (!abst.Ok()) {
      return abst.Error();
    }
    return Term(abst.Value());
  }

  core::ParseResult<Appl> appl = ParseAppl(input);
  if (!appl.Ok()) {
    return appl.Error();
  }
  return Term(appl.Value());
}
}  // namespace named
}  // namespace lhat
