#include "parser.h"
#include "printer.h"

#include <string>

#include "gmock/gmock.h"
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
