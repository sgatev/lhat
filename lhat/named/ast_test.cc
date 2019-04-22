#include "lhat/named/parse.h"
#include "lhat/named/printer.h"

#include <sstream>
#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ x y) (u v))";
  std::istringstream expr_stream(expr);
  const util::ErrorOr<Term> term = Parse(&expr_stream);
  EXPECT_TRUE(term.Ok());
  std::string out;
  Printer::Print(term.Value(), &out);
  EXPECT_EQ(out, expr);
}
}  // namespace
}  // namespace named
}  // namespace lhat
