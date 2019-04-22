#include "lhat/named/parse.h"

#include "lhat/util/parse.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

util::ErrorOr<std::string> ParseName(std::istream* input) {
  std::string name;
  while (!input->eof() && input->peek() >= 0 && !IsSpecial(input->peek())) {
    name.push_back(input->get());
  }
  return name;
}

util::ErrorOr<Var> ParseVar(std::istream* input) {
  util::ErrorOr<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return var_name.Error();
  }
  return Var(var_name.Value());
}

util::ErrorOr<Abst> ParseAbst(std::istream* input) {
  // discard '^'
  input->get();

  util::DiscardWhitespace(input);

  // parse var name
  util::ErrorOr<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return var_name.Error();
  }

  util::DiscardWhitespace(input);

  // parse body term
  util::ErrorOr<Term> term = Parse(input);
  if (!term.Ok()) {
    return term.Error();
  }

  util::DiscardWhitespace(input);

  // discard ')'
  input->get();

  return Abst(var_name.Value(), term.Value());
}

util::ErrorOr<Appl> ParseAppl(std::istream* input) {
  util::DiscardWhitespace(input);

  // parse func term
  util::ErrorOr<Term> left = Parse(input);
  if (!left.Ok()) {
    return left.Error();
  }

  util::DiscardWhitespace(input);

  // parse arg term
  util::ErrorOr<Term> right = Parse(input);
  if (!right.Ok()) {
    return right.Error();
  }

  util::DiscardWhitespace(input);

  // discard ')'
  input->get();

  return Appl(left.Value(), right.Value());
}
}  // namespace

util::ErrorOr<Term> Parse(std::istream* input) {
  util::DiscardWhitespace(input);

  if (input->eof()) {
    return util::Error("Failed to parse term: given expression is empty");
  }
  if (input->peek() != '(') {
    util::ErrorOr<Var> var = ParseVar(input);
    if (!var.Ok()) {
      return var.Error();
    }
    return Term(var.Value());
  }

  // discard '('
  input->get();

  util::DiscardWhitespace(input);

  if (input->eof()) {
    return util::Error("Failed to parse term: ( is not closed");
  }

  if (input->peek() == '^') {
    util::ErrorOr<Abst> abst = ParseAbst(input);
    if (!abst.Ok()) {
      return abst.Error();
    }
    return Term(abst.Value());
  }

  util::ErrorOr<Appl> appl = ParseAppl(input);
  if (!appl.Ok()) {
    return appl.Error();
  }
  return Term(appl.Value());
}
}  // namespace named
}  // namespace lhat
