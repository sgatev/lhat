#include "lhat/nameless/parser.h"
#include "lhat/nameless/printer.h"

#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ 0) (^ (0 ((^ 2) 1))))";
  const core::ParseResult<Term> term = Parser::Parse(expr);
  EXPECT_TRUE(term.Ok());
  std::string out;
  Printer::Print(term.Value(), &out);
  EXPECT_EQ(out, expr);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
