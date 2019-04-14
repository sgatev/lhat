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
std::pair<std::string, std::string> ReadCommand(std::string input) {
  return absl::StrSplit(input, absl::MaxSplits(' ', 1));
}

void Run() {
  ConstEnv consts;
  std::string input, command;
  while (true) {
    std::getline(std::cin, input);
    std::tie(command, input) = ReadCommand(input);

    auto with_input_term = [&consts, &input](auto&& handle_term) {
      consts.Resolve(&input);

      const core::ParseResult<named::Term> input_parse_result =
          named::Parse(input);
      if (!input_parse_result.Ok()) {
        std::cout << "Failed to parse term: "
                  << input_parse_result.Error().Message() << std::endl;
        return;
      }
      const named::Term input_term = input_parse_result.Value();

      NameContext free_nctx;
      nameless::Term term = RemoveNames(input_term, &free_nctx);

      handle_term(&term, &free_nctx);
    };

    if (command == "exit") {
      break;
    } else if (command == "def") {
      consts.Set(absl::StrSplit(input, absl::MaxSplits(' ', 1)));
    } else if (command == "alpha-equiv?") {
      consts.Resolve(&input);

      const core::ParseResult<named::Term> first_parse_result =
          named::Parse(input);
      if (!first_parse_result.Ok()) {
        std::cout << "Failed to parse term: "
                  << first_parse_result.Error().Message() << std::endl;
        return;
      }
      const named::Term first_term = first_parse_result.Value();

      input = input.substr(first_parse_result.ConsumedChars());
      const core::ParseResult<named::Term> second_term_result =
          named::Parse(input);
      if (!second_term_result.Ok()) {
        std::cout << "Failed to parse term: "
                  << second_term_result.Error().Message() << std::endl;
        return;
      }
      const named::Term second_term = second_term_result.Value();

      std::cout << std::boolalpha
                << named::IsAlphaEquiv(first_term, second_term) << std::endl;
    } else if (command == "beta-redex?") {
      with_input_term(
          [](const nameless::Term* term, const NameContext* free_nctx) {
            std::cout << std::boolalpha << nameless::IsBetaRedex(*term)
                      << std::endl;
          });
    } else if (command == "beta-normal?") {
      with_input_term(
          [](const nameless::Term* term, const NameContext* free_nctx) {
            std::cout << std::boolalpha << nameless::IsBetaNormalForm(*term)
                      << std::endl;
          });
    } else if (command == "eval-appl") {
      with_input_term([](nameless::Term* term, NameContext* free_nctx) {
        while (nameless::BetaReduceAppl(term)) {
          // Normalize the term.
        }

        const named::Term output_term = AddNames(*term, free_nctx);
        std::string output;
        named::Printer::Print(output_term, &output);
        std::cout << output << std::endl;
      });
    } else {
      std::cout << "Unknown command: " << command << std::endl;
    }
  }
}
}  // namespace
}  // namespace repl
}  // namespace lhat

int main() { lhat::repl::Run(); }
