#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "lhat/nameless/parser.h"
#include "lhat/nameless/printer.h"

namespace lhat::nameless {
namespace {

TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ 0) (^ (0 ((^ 2) 1))))";
  std::istringstream expr_stream(expr);
  const util::ErrorOr<Term> term = Parser::Parse(&expr_stream);
  EXPECT_TRUE(term.Ok());
  std::string out;
  Printer::Print(term.Value(), &out);
  EXPECT_EQ(out, expr);
}

}  // namespace
}  // namespace lhat::nameless
