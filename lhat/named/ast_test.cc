#include "lhat/named/parser.h"
#include "lhat/named/printer.h"

#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ x y) (u v))";
  const Term term = Parser::Parse(expr);
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, expr);
}
}  // namespace
}  // namespace named
}  // namespace lhat
