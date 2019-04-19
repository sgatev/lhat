#include <fstream>
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
#include "lhat/repl/const_env.h"
#include "lhat/repl/parse.h"
#include "lhat/transform/names.h"

#include "absl/strings/ascii.h"
#include "absl/strings/str_split.h"

namespace lhat {
namespace repl {
namespace {
void Def(std::string&& input, ConstEnv* consts) {
  consts->Set(absl::StrSplit(absl::StripAsciiWhitespace(input),
                             absl::MaxSplits(' ', 1)));
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

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term, &free_nctx);

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

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term, &free_nctx);

  std::cout << std::boolalpha << nameless::IsBetaNormalForm(term) << std::endl;
}

void IsHeadNormal(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> input_parse_result = named::Parse(input);
  if (!input_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << input_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term input_term = input_parse_result.Value();

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term, &free_nctx);

  std::cout << std::boolalpha << nameless::IsHeadNormalForm(term) << std::endl;
}

void BetaReduce(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> input_parse_result = named::Parse(input);
  if (!input_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << input_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term input_term = input_parse_result.Value();

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term, &free_nctx);

  nameless::BetaReduceTerm(&term);

  const named::Term output_term = transform::AddNames(term, &free_nctx);
  std::string output;
  named::Printer::Print(output_term, &output);
  std::cout << output << std::endl;
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

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term, &free_nctx);

  while (nameless::BetaReduceAppl(&term)) {
    // Normalize the term.
  }

  const named::Term output_term = transform::AddNames(term, &free_nctx);
  std::string output;
  named::Printer::Print(output_term, &output);
  std::cout << output << std::endl;
}

void EvalNormal(const ConstEnv& consts, std::string&& input) {
  consts.Resolve(&input);

  const core::ParseResult<named::Term> input_parse_result = named::Parse(input);
  if (!input_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << input_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term input_term = input_parse_result.Value();

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term, &free_nctx);

  while (nameless::BetaReduceNormal(&term)) {
    // Normalize the term.
  }

  const named::Term output_term = transform::AddNames(term, &free_nctx);
  std::string output;
  named::Printer::Print(output_term, &output);
  std::cout << output << std::endl;
}

void ExecuteCommand(const std::string& command, ConstEnv* consts,
                    std::string&& input) {
  if (command == "def") {
    Def(std::move(input), consts);
  } else if (command == "alpha-equiv?") {
    IsAlphaEquiv(*consts, std::move(input));
  } else if (command == "beta-redex?") {
    IsBetaRedex(*consts, std::move(input));
  } else if (command == "beta-normal?") {
    IsBetaNormal(*consts, std::move(input));
  } else if (command == "head-normal?") {
    IsHeadNormal(*consts, std::move(input));
  } else if (command == "beta-reduce") {
    BetaReduce(*consts, std::move(input));
  } else if (command == "eval-appl") {
    EvalAppl(*consts, std::move(input));
  } else if (command == "eval-normal") {
    EvalNormal(*consts, std::move(input));
  } else {
    std::cout << "Unknown command: " << command << std::endl;
  }
}

void LoadConstsFromFile(const std::string& file_name, ConstEnv* consts) {
  std::ifstream file(file_name, std::ios::in);
  if (!file.is_open()) {
    return;
  }

  std::string line;
  while (std::getline(file, line)) {
    absl::RemoveExtraAsciiWhitespace(&line);
    if (line.empty()) {
      continue;
    }
    if (line[0] == '#') {
      continue;
    }

    const core::ParseResult<std::string> command = ParseCommand(line);
    if (!command.Ok()) {
      std::cout << "Failed to parse command: " << command.Error().Message()
                << std::endl;
      continue;
    }
    line = line.substr(command.ConsumedChars());

    ExecuteCommand(command.Value(), consts, std::move(line));
  }
  file.close();
}

void Run(int argc, char* argv[]) {
  ConstEnv consts;

  for (int i = 1; i < argc; i += 2) {
    if (std::strcmp(argv[i], "--exec") == 0) {
      if (i + 1 >= argc) {
        std::cout << "Expected path to file" << std::endl;
      } else {
        LoadConstsFromFile(argv[i + 1], &consts);
      }
    } else {
      std::cout << "Unknown flag: " << argv[i] << std::endl;
    }
  }

  std::string input;
  while (true) {
    std::cout << "> ";

    std::getline(std::cin, input);
    absl::RemoveExtraAsciiWhitespace(&input);
    if (input.empty()) {
      continue;
    }

    const core::ParseResult<std::string> command = ParseCommand(input);
    if (!command.Ok()) {
      std::cout << "Failed to parse command: " << command.Error().Message()
                << std::endl;
      continue;
    }
    input = input.substr(command.ConsumedChars());

    if (command.Value() == "exit") {
      break;
    }

    ExecuteCommand(command.Value(), &consts, std::move(input));
  }
}
}  // namespace
}  // namespace repl
}  // namespace lhat

int main(int argc, char* argv[]) { lhat::repl::Run(argc, argv); }
