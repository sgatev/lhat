#include <fstream>
#include <iostream>
#include <string>

#include "lhat/core/line_transform_buf.h"
#include "lhat/named/alpha.h"
#include "lhat/named/ast.h"
#include "lhat/named/parse.h"
#include "lhat/named/printer.h"
#include "lhat/nameless/ast.h"
#include "lhat/nameless/beta.h"
#include "lhat/repl/const_env.h"
#include "lhat/repl/parse.h"
#include "lhat/transform/names.h"

namespace lhat {
namespace repl {
namespace {
void Def(ConstEnv* consts, std::istream* input_stream) {
  const core::ParseResult<std::string> const_name =
      ParseConstName(input_stream);
  if (!const_name.Ok()) {
    std::cout << "Failed to parse const name: " << const_name.Error().Message()
              << std::endl;
    return;
  }

  const core::ParseResult<named::Term> term = named::Parse(input_stream);
  if (!term.Ok()) {
    std::cout << "Failed to parse term: " << term.Error().Message()
              << std::endl;
    return;
  }

  std::string term_str;
  named::Printer::Print(term.Value(), &term_str);
  consts->Set(const_name.Value(), term_str);
}

void IsAlphaEquiv(std::istream* input_stream) {
  const core::ParseResult<named::Term> first_parse_result =
      named::Parse(input_stream);
  if (!first_parse_result.Ok()) {
    std::cout << "Failed to parse term: "
              << first_parse_result.Error().Message() << std::endl;
    return;
  }
  const named::Term first_term = first_parse_result.Value();

  const core::ParseResult<named::Term> second_term_result =
      named::Parse(input_stream);
  if (!second_term_result.Ok()) {
    std::cout << "Failed to parse term: "
              << second_term_result.Error().Message() << std::endl;
    return;
  }
  const named::Term second_term = second_term_result.Value();

  std::cout << std::boolalpha << named::IsAlphaEquiv(first_term, second_term)
            << std::endl;
}

void IsBetaRedex(std::istream* input_stream) {
  const core::ParseResult<named::Term> input_parse_result =
      named::Parse(input_stream);
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

void IsBetaNormal(std::istream* input_stream) {
  const core::ParseResult<named::Term> input_parse_result =
      named::Parse(input_stream);
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

void IsHeadNormal(std::istream* input_stream) {
  const core::ParseResult<named::Term> input_parse_result =
      named::Parse(input_stream);
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

void BetaReduce(std::istream* input_stream) {
  const core::ParseResult<named::Term> input_parse_result =
      named::Parse(input_stream);
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

void EvalAppl(std::istream* input_stream) {
  const core::ParseResult<named::Term> input_parse_result =
      named::Parse(input_stream);
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

void EvalNormal(std::istream* input_stream) {
  const core::ParseResult<named::Term> input_parse_result =
      named::Parse(input_stream);
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
                    std::istream* input_stream) {
  if (command.empty()) {
    return;
  }

  if (command == "def") {
    Def(consts, input_stream);
  } else if (command == "alpha-equiv?") {
    IsAlphaEquiv(input_stream);
  } else if (command == "beta-redex?") {
    IsBetaRedex(input_stream);
  } else if (command == "beta-normal?") {
    IsBetaNormal(input_stream);
  } else if (command == "head-normal?") {
    IsHeadNormal(input_stream);
  } else if (command == "beta-reduce") {
    BetaReduce(input_stream);
  } else if (command == "eval-appl") {
    EvalAppl(input_stream);
  } else if (command == "eval-normal") {
    EvalNormal(input_stream);
  } else {
    std::cout << "Unknown command: " << command << std::endl;
  }
}

void LoadConstsFromFile(const std::string& file_name, ConstEnv* consts) {
  std::ifstream file(file_name, std::ios::in);
  if (!file.is_open()) {
    std::cout << "Failed to open file: " << file_name << std::endl;
    return;
  }

  while (!file.eof()) {
    core::LineTransformBuf const_resolve_buf(
        &file, [consts](std::string* line) { consts->Resolve(line); });
    std::istream input_stream(&const_resolve_buf);

    const core::ParseResult<std::string> command = ParseCommand(&input_stream);
    if (!command.Ok()) {
      std::cout << "Failed to parse command: " << command.Error().Message()
                << std::endl;
      continue;
    }

    ExecuteCommand(command.Value(), consts, &input_stream);
  }
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

  while (true) {
    std::cout << "> ";

    core::LineTransformBuf const_resolve_buf(
        &std::cin, [&consts](std::string* line) { consts.Resolve(line); });
    std::istream input_stream(&const_resolve_buf);

    const core::ParseResult<std::string> command = ParseCommand(&input_stream);
    if (!command.Ok()) {
      std::cout << "Failed to parse command: " << command.Error().Message()
                << std::endl;
      continue;
    }

    if (command.Value() == "exit") {
      break;
    }

    ExecuteCommand(command.Value(), &consts, &input_stream);
  }
}
}  // namespace
}  // namespace repl
}  // namespace lhat

int main(int argc, char* argv[]) { lhat::repl::Run(argc, argv); }
