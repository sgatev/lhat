#include "lhat/named/parse.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

void ParseWhitespace(std::istream* input) {
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
  }
}

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
  // parse '^'
  input->get();

  ParseWhitespace(input);

  // parse var name
  core::ParseResult<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return var_name.Error();
  }

  ParseWhitespace(input);

  // parse body term
  core::ParseResult<Term> term = Parse(input);
  if (!term.Ok()) {
    return term.Error();
  }

  ParseWhitespace(input);

  // parse ')'
  input->get();

  return Abst(var_name.Value(), term.Value());
}

core::ParseResult<Appl> ParseAppl(std::istream* input) {
  ParseWhitespace(input);

  // parse func term
  core::ParseResult<Term> left = Parse(input);
  if (!left.Ok()) {
    return left.Error();
  }

  ParseWhitespace(input);

  // parse arg term
  core::ParseResult<Term> right = Parse(input);
  if (!right.Ok()) {
    return right.Error();
  }

  ParseWhitespace(input);

  // parse ')'
  input->get();

  return Appl(left.Value(), right.Value());
}
}  // namespace

core::ParseResult<Term> Parse(std::istream* input) {
  ParseWhitespace(input);

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

  // parse '('
  input->get();

  ParseWhitespace(input);

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
