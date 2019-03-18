#include "parser.h"

namespace lhat {
namespace nameless {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }
}  // namespace

std::shared_ptr<Term> Parser::Parse(const std::string& expr) {
  if (expr.empty()) {
    return nullptr;
  }
  return Parser(expr).ParseTerm();
}

Parser::Parser(const std::string& expr)
    : expr_(expr), abst_count_(0), idx_(0), done_(false) {}

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

std::shared_ptr<AbstTerm> Parser::ParseAbstTerm() {
  abst_count_++;

  // consume '^'
  Next();

  // consume ' '
  Next();

  std::shared_ptr<Term> term = ParseTerm();

  // consume ')'
  Next();

  abst_count_--;

  return std::make_shared<AbstTerm>(term);
}

std::shared_ptr<ApplTerm> Parser::ParseApplTerm() {
  std::shared_ptr<Term> left = ParseTerm();

  // consume ' '
  Next();

  std::shared_ptr<Term> right = ParseTerm();

  // consume ')'
  Next();

  return std::make_shared<ApplTerm>(left, right);
}

std::shared_ptr<VarTerm> Parser::ParseVarTerm() {
  std::string idx;

  do {
    idx.push_back(expr_[idx_]);
    Next();
  } while (!done_ && !IsSpecial(Peek()));

  return std::make_shared<VarTerm>(std::stoi(idx) - abst_count_);
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
