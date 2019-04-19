#include "lhat/nameless/parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

bool IsWhitespace(char c) { return c == ' ' || c == '\t'; }
}  // namespace

core::ParseResult<Term> Parser::Parse(io::CharReader* reader) {
  return Parser(reader).ParseTerm();
}

Parser::Parser(io::CharReader* reader)
    : reader_(reader), abst_count_(0), idx_(0) {}

core::ParseResult<Term> Parser::ParseTerm() {
  ParseWhitespace();

  if (reader_->Empty()) {
    return core::ParseResult<Term>(
        idx_,
        core::ParseError("Failed to parse term: given expression is empty"));
  }

  if (Peek() != '(') {
    core::ParseResult<Var> var = ParseVar();
    if (!var.Ok()) {
      return core::ParseResult<Term>(var.ConsumedChars(), var.Error());
    }
    return core::ParseResult<Term>(idx_, Term(var.Value()));
  }

  // parse '('
  Next();

  ParseWhitespace();

  if (reader_->Empty()) {
    return core::ParseResult<Term>(
        idx_, core::ParseError("Failed to parse term: ( is not closed"));
  }

  if (Peek() == '^') {
    core::ParseResult<Abst> abst = ParseAbst();
    if (!abst.Ok()) {
      return core::ParseResult<Term>(abst.ConsumedChars(), abst.Error());
    }
    return core::ParseResult<Term>(idx_, Term(abst.Value()));
  }

  core::ParseResult<Appl> appl = ParseAppl();
  if (!appl.Ok()) {
    return core::ParseResult<Term>(appl.ConsumedChars(), appl.Error());
  }
  return core::ParseResult<Term>(idx_, Term(appl.Value()));
}

core::ParseResult<Abst> Parser::ParseAbst() {
  abst_count_++;

  // parse '^'
  Next();

  ParseWhitespace();

  // parse body term
  const core::ParseResult<Term> body = ParseTerm();
  if (!body.Ok()) {
    return core::ParseResult<Abst>(body.ConsumedChars(), body.Error());
  }

  ParseWhitespace();

  // parse ')'
  Next();

  abst_count_--;

  return core::ParseResult<Abst>(idx_, Abst(body.Value()));
}

core::ParseResult<Appl> Parser::ParseAppl() {
  ParseWhitespace();

  // parse func term
  const core::ParseResult<Term> func = ParseTerm();
  if (!func.Ok()) {
    return core::ParseResult<Appl>(func.ConsumedChars(), func.Error());
  }

  ParseWhitespace();

  // parse arg term
  const core::ParseResult<Term> arg = ParseTerm();
  if (!arg.Ok()) {
    return core::ParseResult<Appl>(arg.ConsumedChars(), arg.Error());
  }

  ParseWhitespace();

  // consume ')'
  Next();

  return core::ParseResult<Appl>(idx_, Appl(func.Value(), arg.Value()));
}

core::ParseResult<Var> Parser::ParseVar() {
  std::string idx;
  while (!reader_->Empty() && !IsSpecial(Peek())) {
    idx.push_back(Peek());
    Next();
  }
  return core::ParseResult<Var>(idx_, Var(std::stoi(idx) - abst_count_));
}

void Parser::ParseWhitespace() {
  while (!reader_->Empty() && IsWhitespace(reader_->Peek())) {
    idx_++;
    reader_->Next();
  }
}

char Parser::Peek() { return reader_->Peek(); }

void Parser::Next() {
  idx_++;
  reader_->Next();
}
}  // namespace nameless
}  // namespace lhat
