#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

#include "named/ast.h"
#include "named/parser.h"
#include "nameless/ast.h"
#include "nameless/printer.h"
#include "nameless/reduce.h"
#include "names.h"

namespace lhat {
namespace {
bool EqualsIgnoreCase(const std::string& s1, const std::string& s2) {
  if (s1.size() != s2.size()) {
    return false;
  }
  return std::equal(s1.begin(), s1.end(), s2.begin(),
                    [](char c1, char c2) -> bool {
                      return std::toupper(c1) == std::toupper(c2);
                    });
}

bool ShouldExit(const std::string& input) {
  return EqualsIgnoreCase(input, "exit") || EqualsIgnoreCase(input, "quit");
}

void Run() {
  std::string input;
  while (true) {
    std::getline(std::cin, input);

    if (ShouldExit(input)) {
      break;
    }

    const named::Term input_term = named::Parser::Parse(input);
    nameless::Term term = StripNames(input_term);
    while (!nameless::IsNormalForm(term)) {
      nameless::BetaReduceSubTerms(&term);
    }

    std::string output;
    nameless::Printer::Print(term, &output);
    std::cout << output << std::endl;
  }
}
}  // namespace
}  // namespace lhat

int main() { lhat::Run(); }
