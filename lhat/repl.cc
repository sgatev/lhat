#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "named/ast.h"
#include "named/parser.h"
#include "named/printer.h"
#include "nameless/ast.h"
#include "nameless/reduce.h"
#include "names.h"

#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace lhat {
namespace {
void Run() {
  const std::regex var_name_regex("\\$[a-zA-Z0-9]+", std::regex::extended);

  std::string input;
  std::unordered_map<std::string, std::string> vars;
  while (true) {
    std::getline(std::cin, input);

    std::vector<std::string> input_parts =
        absl::StrSplit(input, absl::MaxSplits(' ', 1));
    const std::string& command = input_parts[0];

    if (command == "exit") {
      break;
    } else if (command == "def") {
      input_parts = absl::StrSplit(input_parts[1], absl::MaxSplits(' ', 1));
      vars[input_parts[0]] = input_parts[1];
    } else {
      std::smatch var_matches;
      if (std::regex_search(input, var_matches, var_name_regex)) {
        std::vector<std::pair<const absl::string_view, std::string>>
            var_replacements;
        for (int i = 0; i < var_matches.size(); i++) {
          std::string var_name(var_matches.str(i));
          var_replacements.push_back({var_name, vars[var_name.substr(1)]});
        }
        absl::StrReplaceAll(var_replacements, &input);
      }

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
