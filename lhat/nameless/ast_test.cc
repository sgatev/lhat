#include "lhat/nameless/parser.h"
#include "lhat/nameless/printer.h"

#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ 0) (^ (0 ((^ 2) 1))))";
  const Term term = Parser::Parse(expr);
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, expr);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
