#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "lhat/named/ast.h"
#include "lhat/named/parser.h"
#include "lhat/named/printer.h"
#include "lhat/nameless/ast.h"
#include "lhat/nameless/reduce.h"
#include "lhat/names.h"
#include "lhat/repl/const_env.h"

#include "absl/strings/str_split.h"

namespace lhat {
namespace repl {
namespace {
void Run() {
  ConstEnv consts;
  std::string input;
  while (true) {
    std::getline(std::cin, input);

    const std::vector<std::string> input_parts =
        absl::StrSplit(input, absl::MaxSplits(' ', 1));
    const std::string& command = input_parts[0];

    if (command == "exit") {
      break;
    } else if (command == "def") {
      consts.Set(absl::StrSplit(input_parts[1], absl::MaxSplits(' ', 1)));
    } else if (command == "beta-redex?") {
      input = input_parts[1];

      consts.Resolve(&input);
      const named::Term input_term = named::Parser::Parse(input);

      NameContext free_nctx;
      nameless::Term term = RemoveNames(input_term, &free_nctx);

      std::cout << std::boolalpha << nameless::IsBetaRedex(term) << std::endl;
    } else if (command == "normal-form?") {
      input = input_parts[1];

      consts.Resolve(&input);
      const named::Term input_term = named::Parser::Parse(input);

      NameContext free_nctx;
      nameless::Term term = RemoveNames(input_term, &free_nctx);

      std::cout << std::boolalpha << nameless::IsNormalForm(term) << std::endl;
    } else {
      consts.Resolve(&input);
      const named::Term input_term = named::Parser::Parse(input);

      NameContext free_nctx;
      nameless::Term term = RemoveNames(input_term, &free_nctx);

      while (nameless::BetaReduceAppl(&term)) {
        // Reduce the term.
      }

      const named::Term output_term = AddNames(term, &free_nctx);
      std::string output;
      named::Printer::Print(output_term, &output);
      std::cout << output << std::endl;
    }
  }
}
}  // namespace
}  // namespace repl
}  // namespace lhat

int main() { lhat::repl::Run(); }
