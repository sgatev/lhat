#include "parser.h"
#include "printer.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ x y) (u v))";
  const std::shared_ptr<Term> term = Parser::Parse(expr);
  EXPECT_EQ(Printer::Print(term), expr);
}
}  // namespace
}  // namespace named
}  // namespace lhat
