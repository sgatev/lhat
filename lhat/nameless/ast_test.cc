#include "parser.h"
#include "printer.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ 0) (^ (0 ((^ 2) 1))))";
  const std::shared_ptr<Term> term = Parser::Parse(expr);
  EXPECT_EQ(Printer::Print(term), expr);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
