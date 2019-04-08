#include "printer.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Printer, Var) {
  const Term term = Var("x");
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "x");
}

TEST(Printer, Abst) {
  const Term term = Abst("x", Var("y"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ x y)");
}

TEST(Printer, Appl) {
  const Term term = Appl(Var("x"), Var("y"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(x y)");
}

TEST(Printer, Complex) {
  const Term term = Appl(Abst("x", Var("y")), Appl(Var("u"), Var("v")));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "((^ x y) (u v))");
}
}  // namespace
}  // namespace named
}  // namespace lhat
