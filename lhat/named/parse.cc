#include "lhat/named/parse.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

bool IsWhitespace(char c) { return c == ' ' || c == '\t'; }

int ParseWhitespace(io::CharReader* reader) {
  int idx = 0;
  while (!reader->Empty() && IsWhitespace(reader->Peek())) {
    reader->Next();
    idx++;
  }
  return idx;
}

core::ParseResult<std::string> ParseName(io::CharReader* reader) {
  std::string name;
  int idx = 0;
  while (!reader->Empty() && !IsSpecial(reader->Peek())) {
    name.push_back(reader->Peek());
    reader->Next();
    idx++;
  }
  return core::ParseResult<std::string>(idx, name);
}

core::ParseResult<Var> ParseVar(io::CharReader* reader) {
  core::ParseResult<std::string> var_name = ParseName(reader);
  if (!var_name.Ok()) {
    return core::ParseResult<Var>(var_name.ConsumedChars(), var_name.Error());
  }
  return core::ParseResult<Var>(var_name.ConsumedChars(),
                                Var(var_name.Value()));
}

core::ParseResult<Abst> ParseAbst(io::CharReader* reader) {
  // parse '^'
  int idx = 1;
  reader->Next();

  idx += ParseWhitespace(reader);

  // parse var name
  core::ParseResult<std::string> var_name = ParseName(reader);
  if (!var_name.Ok()) {
    return core::ParseResult<Abst>(idx + var_name.ConsumedChars(),
                                   var_name.Error());
  }
  idx += var_name.ConsumedChars();

  idx += ParseWhitespace(reader);

  // parse body term
  core::ParseResult<Term> term = Parse(reader);
  if (!term.Ok()) {
    return core::ParseResult<Abst>(idx + term.ConsumedChars(), term.Error());
  }
  idx += term.ConsumedChars();

  idx += ParseWhitespace(reader);

  // parse ')'
  idx++;
  reader->Next();

  return core::ParseResult<Abst>(idx, Abst(var_name.Value(), term.Value()));
}

core::ParseResult<Appl> ParseAppl(io::CharReader* reader) {
  int idx = ParseWhitespace(reader);

  // parse func term
  core::ParseResult<Term> left = Parse(reader);
  if (!left.Ok()) {
    return core::ParseResult<Appl>(idx + left.ConsumedChars(), left.Error());
  }
  idx = left.ConsumedChars();

  idx += ParseWhitespace(reader);

  // parse arg term
  core::ParseResult<Term> right = Parse(reader);
  if (!right.Ok()) {
    return core::ParseResult<Appl>(idx + right.ConsumedChars(), right.Error());
  }
  idx += right.ConsumedChars();

  idx += ParseWhitespace(reader);

  // parse ')'
  idx++;
  reader->Next();

  return core::ParseResult<Appl>(idx, Appl(left.Value(), right.Value()));
}
}  // namespace

core::ParseResult<Term> Parse(io::CharReader* reader) {
  int idx = ParseWhitespace(reader);

  if (reader->Empty()) {
    return core::ParseResult<Term>(
        idx,
        core::ParseError("Failed to parse term: given expression is empty"));
  }
  if (reader->Peek() != '(') {
    core::ParseResult<Var> var = ParseVar(reader);
    if (!var.Ok()) {
      return core::ParseResult<Term>(idx + var.ConsumedChars(), var.Error());
    }
    return core::ParseResult<Term>(idx + var.ConsumedChars(), var.Value());
  }

  // parse '('
  idx++;
  reader->Next();

  idx += ParseWhitespace(reader);

  if (reader->Empty()) {
    return core::ParseResult<Term>(
        idx, core::ParseError("Failed to parse term: ( is not closed"));
  }

  if (reader->Peek() == '^') {
    core::ParseResult<Abst> abst = ParseAbst(reader);
    idx += abst.ConsumedChars();
    if (!abst.Ok()) {
      return core::ParseResult<Term>(idx, abst.Error());
    }
    return core::ParseResult<Term>(idx, abst.Value());
  }

  core::ParseResult<Appl> appl = ParseAppl(reader);
  idx += appl.ConsumedChars();
  if (!appl.Ok()) {
    return core::ParseResult<Term>(appl.ConsumedChars(), appl.Error());
  }
  return core::ParseResult<Term>(idx, appl.Value());
}
}  // namespace named
}  // namespace lhat
