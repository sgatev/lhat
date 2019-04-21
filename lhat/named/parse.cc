#include "lhat/named/parse.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

bool IsWhitespace(char c) { return c == ' ' || c == '\t'; }

int ParseWhitespace(std::istream* input) {
  int idx = 0;
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
    idx++;
  }
  return idx;
}

core::ParseResult<std::string> ParseName(std::istream* input) {
  std::string name;
  int idx = 0;
  while (!input->eof() && input->peek() >= 0 && !IsSpecial(input->peek())) {
    name.push_back(input->get());
    idx++;
  }
  return core::ParseResult<std::string>(idx, name);
}

core::ParseResult<Var> ParseVar(std::istream* input) {
  core::ParseResult<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return core::ParseResult<Var>(var_name.ConsumedChars(), var_name.Error());
  }
  return core::ParseResult<Var>(var_name.ConsumedChars(),
                                Var(var_name.Value()));
}

core::ParseResult<Abst> ParseAbst(std::istream* input) {
  // parse '^'
  int idx = 1;
  input->get();

  idx += ParseWhitespace(input);

  // parse var name
  core::ParseResult<std::string> var_name = ParseName(input);
  if (!var_name.Ok()) {
    return core::ParseResult<Abst>(idx + var_name.ConsumedChars(),
                                   var_name.Error());
  }
  idx += var_name.ConsumedChars();

  idx += ParseWhitespace(input);

  // parse body term
  core::ParseResult<Term> term = Parse(input);
  if (!term.Ok()) {
    return core::ParseResult<Abst>(idx + term.ConsumedChars(), term.Error());
  }
  idx += term.ConsumedChars();

  idx += ParseWhitespace(input);

  // parse ')'
  idx++;
  input->get();

  return core::ParseResult<Abst>(idx, Abst(var_name.Value(), term.Value()));
}

core::ParseResult<Appl> ParseAppl(std::istream* input) {
  int idx = ParseWhitespace(input);

  // parse func term
  core::ParseResult<Term> left = Parse(input);
  if (!left.Ok()) {
    return core::ParseResult<Appl>(idx + left.ConsumedChars(), left.Error());
  }
  idx = left.ConsumedChars();

  idx += ParseWhitespace(input);

  // parse arg term
  core::ParseResult<Term> right = Parse(input);
  if (!right.Ok()) {
    return core::ParseResult<Appl>(idx + right.ConsumedChars(), right.Error());
  }
  idx += right.ConsumedChars();

  idx += ParseWhitespace(input);

  // parse ')'
  idx++;
  input->get();

  return core::ParseResult<Appl>(idx, Appl(left.Value(), right.Value()));
}
}  // namespace

core::ParseResult<Term> Parse(std::istream* input) {
  int idx = ParseWhitespace(input);

  if (input->eof()) {
    return core::ParseResult<Term>(
        idx,
        core::ParseError("Failed to parse term: given expression is empty"));
  }
  if (input->peek() != '(') {
    core::ParseResult<Var> var = ParseVar(input);
    if (!var.Ok()) {
      return core::ParseResult<Term>(idx + var.ConsumedChars(), var.Error());
    }
    return core::ParseResult<Term>(idx + var.ConsumedChars(), var.Value());
  }

  // parse '('
  idx++;
  input->get();

  idx += ParseWhitespace(input);

  if (input->eof()) {
    return core::ParseResult<Term>(
        idx, core::ParseError("Failed to parse term: ( is not closed"));
  }

  if (input->peek() == '^') {
    core::ParseResult<Abst> abst = ParseAbst(input);
    idx += abst.ConsumedChars();
    if (!abst.Ok()) {
      return core::ParseResult<Term>(idx, abst.Error());
    }
    return core::ParseResult<Term>(idx, abst.Value());
  }

  core::ParseResult<Appl> appl = ParseAppl(input);
  idx += appl.ConsumedChars();
  if (!appl.Ok()) {
    return core::ParseResult<Term>(appl.ConsumedChars(), appl.Error());
  }
  return core::ParseResult<Term>(idx, appl.Value());
}
}  // namespace named
}  // namespace lhat
