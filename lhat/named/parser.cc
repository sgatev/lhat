#include "parser.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

std::shared_ptr<Term> Parser::Parse(const std::string& expr) {
  if (expr.empty()) {
    return nullptr;
  }
  return Parser(expr).ParseTerm();
}

Parser::Parser(const std::string& expr) : expr_(expr), idx_(0), done_(false) {}

std::shared_ptr<Term> Parser::ParseTerm() {
  if (Peek() != '(') {
    return ParseVarTerm();
  }

  // consume '('
  Next();

  if (Peek() == '^') {
    return ParseAbstTerm();
  }

  return ParseApplTerm();
}

std::shared_ptr<Term> Parser::ParseAbstTerm() {
  // consume '^'
  Next();

  // consume ' '
  Next();

  const std::string& var_name = ParseName();

  // consume ' '
  Next();

  const std::shared_ptr<Term> term = ParseTerm();

  // consume ')'
  Next();

  return AbstTerm::Make(var_name, term);
}

std::shared_ptr<Term> Parser::ParseApplTerm() {
  const std::shared_ptr<Term> left = ParseTerm();

  // consume ' '
  Next();

  const std::shared_ptr<Term> right = ParseTerm();

  // consume ')'
  Next();

  return ApplTerm::Make(left, right);
}

std::shared_ptr<Term> Parser::ParseVarTerm() {
  const std::string& name = ParseName();
  return VarTerm::Make(name);
}

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
