#include <iostream>
#include <string>
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

    std::vector<std::string> input_parts =
        absl::StrSplit(input, absl::MaxSplits(' ', 1));
    const std::string& command = input_parts[0];

    if (command == "exit") {
      break;
    } else if (command == "def") {
      consts.Set(absl::StrSplit(input_parts[1], absl::MaxSplits(' ', 1)));
    } else {
      consts.Resolve(&input);

      const named::Term input_term = named::Parser::Parse(input);
      nameless::Term term = RemoveNames(input_term);
      while (!nameless::IsNormalForm(term)) {
        nameless::BetaReduceSubTerms(&term);
      }

      const named::Term output_term = AddNames(term);
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
