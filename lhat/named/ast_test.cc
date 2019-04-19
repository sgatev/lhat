#include "lhat/named/parse.h"
#include "lhat/named/printer.h"

#include <sstream>
#include <string>

#include "lhat/io/char_reader.h"

#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Ast, ParsePrintConsistency) {
  const std::string expr = "((^ x y) (u v))";
  std::istringstream expr_stream(expr);
  io::CharReader expr_reader(&expr_stream);
  const core::ParseResult<Term> term = Parse(&expr_reader);
  EXPECT_TRUE(term.Ok());
  std::string out;
  Printer::Print(term.Value(), &out);
  EXPECT_EQ(out, expr);
}
}  // namespace
}  // namespace named
}  // namespace lhat
