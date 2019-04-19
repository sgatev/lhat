#include "lhat/nameless/parser.h"
#include "lhat/nameless/printer.h"

#include <sstream>
#include <string>

#include "lhat/io/char_reader.h"

#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ 0) (^ (0 ((^ 2) 1))))";
  std::istringstream expr_stream(expr);
  io::CharReader expr_reader(&expr_stream);
  const core::ParseResult<Term> term = Parser::Parse(&expr_reader);
  EXPECT_TRUE(term.Ok());
  std::string out;
  Printer::Print(term.Value(), &out);
  EXPECT_EQ(out, expr);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
