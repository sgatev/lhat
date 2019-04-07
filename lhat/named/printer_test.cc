#include "printer.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Printer, Var) {
  const Term term(Var("x"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "x");
}

TEST(Printer, Abst) {
  const Term term(Abst("x", Term(Var("y"))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ x y)");
}

TEST(Printer, Appl) {
  const Term term(Appl(Term(Var("x")), Term(Var("y"))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(x y)");
}

TEST(Printer, Complex) {
  const Term term(Appl(Term(Abst("x", Term(Var("y")))),
                       Term(Appl(Term(Var("u")), Term(Var("v"))))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "((^ x y) (u v))");
}
}  // namespace
}  // namespace named
}  // namespace lhat
