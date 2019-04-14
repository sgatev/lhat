#include "lhat/nameless/parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

core::ParseResult<Term> Parser::Parse(const std::string_view expr) {
  return Parser(expr).ParseTerm();
}

Parser::Parser(const std::string_view expr)
    : expr_(expr), abst_count_(0), idx_(0), done_(false) {}

core::ParseResult<Term> Parser::ParseTerm() {
  ParseWhitespace();

  if (idx_ >= expr_.size()) {
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

  if (idx_ >= expr_.size()) {
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
  while (!done_ && !IsSpecial(Peek())) {
    idx.push_back(expr_[idx_]);
    Next();
  }
  return core::ParseResult<Var>(idx_, Var(std::stoi(idx) - abst_count_));
}

void Parser::ParseWhitespace() {
  while (idx_ < expr_.size() && (expr_[idx_] == ' ' || expr_[idx_] == '\t')) {
    idx_++;
  }
}

char Parser::Peek() { return expr_[idx_]; }

void Parser::Next() {
  idx_++;

  if (idx_ >= expr_.size()) {
    done_ = true;
  }
}
}  // namespace nameless
}  // namespace lhat
