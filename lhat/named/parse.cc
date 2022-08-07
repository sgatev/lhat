#include "lhat/named/parse.h"

#include <cctype>

#include "lhat/util/parse.h"

namespace lhat::named {
namespace {

bool IsSpecial(char c) {
  return c == '(' || c == ')' || c == '^' || std::isspace(c);
}

util::ErrorOr<std::string> ParseName(std::istream* input) {
  std::string name;
  while (!input->eof() && input->peek() >= 0 && !IsSpecial(input->peek())) {
    name.push_back(input->get());
  }
  return name;
}

util::ErrorOr<Var> ParseVar(std::istream* input) {
  util::ErrorOr<std::string> var_name = ParseName(input);
  RETURN_IF_ERROR(var_name);
  return Var(var_name.Value());
}

util::ErrorOr<Abst> ParseAbst(std::istream* input) {
  // discard '^'
  input->get();

  util::DiscardWhitespace(input);

  // parse var name
  util::ErrorOr<std::string> var_name = ParseName(input);
  RETURN_IF_ERROR(var_name);

  util::DiscardWhitespace(input);

  // parse body term
  util::ErrorOr<Term> term = Parse(input);
  RETURN_IF_ERROR(term);

  util::DiscardWhitespace(input);

  // discard ')'
  input->get();

  return Abst(var_name.Value(), term.Value());
}

util::ErrorOr<Appl> ParseAppl(std::istream* input) {
  util::DiscardWhitespace(input);

  // parse func term
  util::ErrorOr<Term> func = Parse(input);
  RETURN_IF_ERROR(func);

  util::DiscardWhitespace(input);

  // parse arg term
  util::ErrorOr<Term> arg = Parse(input);
  RETURN_IF_ERROR(arg);

  util::DiscardWhitespace(input);

  // discard ')'
  input->get();

  return Appl(func.Value(), arg.Value());
}

}  // namespace

util::ErrorOr<Term> Parse(std::istream* input) {
  util::DiscardWhitespace(input);

  if (input->eof()) {
    return util::Error("Failed to parse term: given expression is empty");
  }
  if (input->peek() != '(') {
    util::ErrorOr<Var> var = ParseVar(input);
    RETURN_IF_ERROR(var);
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
    RETURN_IF_ERROR(abst);
    return Term(abst.Value());
  }

  util::ErrorOr<Appl> appl = ParseAppl(input);
  RETURN_IF_ERROR(appl);
  return Term(appl.Value());
}

}  // namespace lhat::named
