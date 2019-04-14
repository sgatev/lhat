#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#include "lhat/named/alpha.h"
#include "lhat/named/ast.h"
#include "lhat/named/parse.h"
#include "lhat/named/printer.h"
#include "lhat/nameless/ast.h"
#include "lhat/nameless/beta.h"
#include "lhat/names.h"
#include "lhat/repl/const_env.h"

#include "absl/strings/str_split.h"

namespace lhat {
namespace repl {
namespace {
// Trim the prefix of spaces in s.
void TrimPrefix(std::string* s) {
  s->erase(s->begin(), std::find_if(s->begin(), s->end(),
                                    [](int ch) { return !std::isspace(ch); }));
}

// Reads a pair of command and arguments string from input.
std::pair<std::string, std::string> ReadCommand(std::string&& input) {
  return absl::StrSplit(input, absl::MaxSplits(' ', 1));
}

void Def(std::string&& input, ConstEnv* consts) {
  consts->Set(absl::StrSplit(input, absl::MaxSplits(' ', 1)));
}

void IsAlphaEquiv(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> first_parse_result = named::Parse(input);
  if (!first_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << first_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term first_term = first_parse_result.Value();

  input = input.substr(first_parse_result.ConsumedChars());
  const core::ParseResult<named::Term> second_term_result = named::Parse(input);
  if (!second_term_result.Ok()) {
    std::cout << "Failed to parse term: "
              << second_term_result.Error().Message() << std::endl;
    return;
  }
  const named::Term second_term = second_term_result.Value();

  std::cout << std::boolalpha << named::IsAlphaEquiv(first_term, second_term)
            << std::endl;
}

void IsBetaRedex(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> input_parse_result = named::Parse(input);
  if (!input_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << input_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term input_term = input_parse_result.Value();

  NameContext free_nctx;
  nameless::Term term = RemoveNames(input_term, &free_nctx);

  std::cout << std::boolalpha << nameless::IsBetaRedex(term) << std::endl;
}

void IsBetaNormal(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> input_parse_result = named::Parse(input);
  if (!input_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << input_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term input_term = input_parse_result.Value();

  NameContext free_nctx;
  nameless::Term term = RemoveNames(input_term, &free_nctx);

  std::cout << std::boolalpha << nameless::IsBetaNormalForm(term) << std::endl;
}

void EvalAppl(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> input_parse_result = named::Parse(input);
  if (!input_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << input_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term input_term = input_parse_result.Value();

  NameContext free_nctx;
  nameless::Term term = RemoveNames(input_term, &free_nctx);

  while (nameless::BetaReduceAppl(&term)) {
    // Normalize the term.
  }

  const named::Term output_term = AddNames(term, &free_nctx);
  std::string output;
  named::Printer::Print(output_term, &output);
  std::cout << output << std::endl;
}

void Run() {
  ConstEnv consts;
  std::string input, command;
  while (true) {
    std::cout << "> ";

    std::getline(std::cin, input);
    TrimPrefix(&input);
    if (input.empty()) {
      continue;
    }

    std::tie(command, input) = ReadCommand(std::move(input));
    if (command == "exit") {
      break;
    } else if (command == "def") {
      Def(std::move(input), &consts);
    } else if (command == "alpha-equiv?") {
      IsAlphaEquiv(consts, std::move(input));
    } else if (command == "beta-redex?") {
      IsBetaRedex(consts, std::move(input));
    } else if (command == "beta-normal?") {
      IsBetaNormal(consts, std::move(input));
    } else if (command == "eval-appl") {
      EvalAppl(consts, std::move(input));
    } else {
      std::cout << "Unknown command: " << command << std::endl;
    }
  }
}
}  // namespace
}  // namespace repl
}  // namespace lhat

int main() { lhat::repl::Run(); }
