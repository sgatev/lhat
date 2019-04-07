#include "parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

Term Parser::Parse(const std::string& expr) { return Parser(expr).ParseTerm(); }

Parser::Parser(const std::string& expr)
    : expr_(expr), abst_count_(0), idx_(0), done_(false) {}

Term Parser::ParseTerm() {
  if (Peek() != '(') {
    return Term(ParseVar());
  }

  // consume '('
  Next();

  if (Peek() == '^') {
    return Term(ParseAbst());
  }

  return Term(ParseAppl());
}

Abst Parser::ParseAbst() {
  abst_count_++;

  // consume '^'
  Next();

  // consume ' '
  Next();

  const Term body = ParseTerm();

  // consume ')'
  Next();

  abst_count_--;

  return Abst(body);
}

Appl Parser::ParseAppl() {
  const Term func = ParseTerm();

  // consume ' '
  Next();

  const Term arg = ParseTerm();

  // consume ')'
  Next();

  return Appl(func, arg);
}

Var Parser::ParseVar() {
  std::string idx;

  do {
    idx.push_back(expr_[idx_]);
    Next();
  } while (!done_ && !IsSpecial(Peek()));

  return Var(std::stoi(idx) - abst_count_);
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
