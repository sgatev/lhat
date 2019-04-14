#include "lhat/named/parse.h"

namespace lhat {
namespace named {
namespace {
bool IsSpecial(char c) { return c == '(' || c == ')' || c == '^' || c == ' '; }

int ParseWhitespace(const absl::string_view expr) {
  int idx = 0;
  while (idx < expr.size() && (expr[idx] == ' ' || expr[idx] == '\t')) {
    idx++;
  }
  return idx;
}

core::ParseResult<std::string> ParseName(const absl::string_view expr) {
  std::string name;
  int idx = 0;
  while (idx < expr.size() && !IsSpecial(expr[idx])) {
    name.push_back(expr[idx]);
    idx++;
  }
  return core::ParseResult<std::string>(idx, name);
}

core::ParseResult<Var> ParseVar(const absl::string_view expr) {
  core::ParseResult<std::string> var_name = ParseName(expr);
  if (!var_name.Ok()) {
    return core::ParseResult<Var>(var_name.ConsumedChars(), var_name.Error());
  }
  return core::ParseResult<Var>(var_name.ConsumedChars(),
                                Var(var_name.Value()));
}

core::ParseResult<Abst> ParseAbst(const absl::string_view expr) {
  // parse '^'
  int idx = 1;

  idx += ParseWhitespace(expr.substr(idx));

  // parse var name
  core::ParseResult<std::string> var_name = ParseName(expr.substr(idx));
  if (!var_name.Ok()) {
    return core::ParseResult<Abst>(idx + var_name.ConsumedChars(),
                                   var_name.Error());
  }
  idx += var_name.ConsumedChars();

  idx += ParseWhitespace(expr.substr(idx));

  // parse body term
  core::ParseResult<Term> term = Parse(expr.substr(idx));
  if (!term.Ok()) {
    return core::ParseResult<Abst>(idx + term.ConsumedChars(), term.Error());
  }
  idx += term.ConsumedChars();

  idx += ParseWhitespace(expr.substr(idx));

  // parse ')'
  idx++;

  return core::ParseResult<Abst>(idx, Abst(var_name.Value(), term.Value()));
}

core::ParseResult<Appl> ParseAppl(const absl::string_view expr) {
  int idx = ParseWhitespace(expr);

  // parse func term
  core::ParseResult<Term> left = Parse(expr.substr(idx));
  if (!left.Ok()) {
    return core::ParseResult<Appl>(idx + left.ConsumedChars(), left.Error());
  }
  idx = left.ConsumedChars();

  idx += ParseWhitespace(expr.substr(idx));

  // parse arg term
  core::ParseResult<Term> right = Parse(expr.substr(idx));
  if (!right.Ok()) {
    return core::ParseResult<Appl>(idx + right.ConsumedChars(), right.Error());
  }
  idx += right.ConsumedChars();

  idx += ParseWhitespace(expr.substr(idx));

  // parse ')'
  idx++;

  return core::ParseResult<Appl>(idx, Appl(left.Value(), right.Value()));
}
}  // namespace

core::ParseResult<Term> Parse(const absl::string_view expr) {
  int idx = ParseWhitespace(expr);

  if (idx >= expr.size()) {
    return core::ParseResult<Term>(
        idx,
        core::ParseError("Failed to parse term: given expression is empty"));
  }
  if (expr[idx] != '(') {
    core::ParseResult<Var> var = ParseVar(expr);
    if (!var.Ok()) {
      return core::ParseResult<Term>(idx + var.ConsumedChars(), var.Error());
    }
    return core::ParseResult<Term>(idx + var.ConsumedChars(), var.Value());
  }

  // parse '('
  idx++;

  idx += ParseWhitespace(expr.substr(idx));

  if (idx >= expr.size()) {
    return core::ParseResult<Term>(
        idx, core::ParseError("Failed to parse term: ( is not closed"));
  }

  if (expr[idx] == '^') {
    core::ParseResult<Abst> abst = ParseAbst(expr.substr(idx));
    idx += abst.ConsumedChars();
    if (!abst.Ok()) {
      return core::ParseResult<Term>(idx, abst.Error());
    }
    return core::ParseResult<Term>(idx, abst.Value());
  }

  core::ParseResult<Appl> appl = ParseAppl(expr.substr(idx));
  idx += appl.ConsumedChars();
  if (!appl.Ok()) {
    return core::ParseResult<Term>(appl.ConsumedChars(), appl.Error());
  }
  return core::ParseResult<Term>(idx, appl.Value());
}
}  // namespace named
}  // namespace lhat
