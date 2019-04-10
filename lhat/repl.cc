#include <iostream>
#include <string>
#include <vector>

#include "const_env.h"
#include "named/ast.h"
#include "named/parser.h"
#include "named/printer.h"
#include "nameless/ast.h"
#include "nameless/reduce.h"
#include "names.h"

#include "absl/strings/str_split.h"

namespace lhat {
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
      input_parts = absl::StrSplit(input_parts[1], absl::MaxSplits(' ', 1));
      consts.Set(input_parts[0], input_parts[1]);
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
}  // namespace lhat

int main() { lhat::Run(); }
