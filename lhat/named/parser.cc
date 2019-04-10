#include "lhat/named/parser.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

Term Parser::Parse(const std::string& expr) { return Parser(expr).ParseTerm(); }

Parser::Parser(const std::string& expr) : expr_(expr), idx_(0), done_(false) {}

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
  // consume '^'
  Next();

  // consume ' '
  Next();

  std::string var_name = ParseName();

  // consume ' '
  Next();

  Term term = ParseTerm();

  // consume ')'
  Next();

  return Abst(var_name, term);
}

Appl Parser::ParseAppl() {
  Term left = ParseTerm();

  // consume ' '
  Next();

  Term right = ParseTerm();

  // consume ')'
  Next();

  return Appl(left, right);
}

Var Parser::ParseVar() { return Var(ParseName()); }

std::string Parser::ParseName() {
  std::string name;

  do {
    name.push_back(expr_[idx_]);
    Next();
  } while (!done_ && !IsSpecial(Peek()));

  return name;
}

char Parser::Peek() { return expr_[idx_]; }

void Parser::Next() {
  idx_++;

  if (idx_ >= expr_.size()) {
    done_ = true;
  }
}
}  // namespace named
}  // namespace lhat
