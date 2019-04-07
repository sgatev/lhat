#include "printer.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
TEST(Printer, Var) {
  const Term term(Var(21));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "21");
}

TEST(Printer, AbstFree) {
  const Term term(Abst(Term(Var(1))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ 2)");
}

TEST(Printer, AbstBound) {
  const Term term(Abst(Term(Var(-1))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ 0)");
}

TEST(Printer, MultiAbstBound) {
  const Term term(Abst(Term(Abst(Term(Var(-1))))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ (^ 1))");
}

TEST(Printer, Appl) {
  const Term term(Appl(Term(Var(1)), Term(Var(2))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(1 2)");
}

TEST(Printer, Complex) {
  const Term term(
      Appl(Term(Abst(Term(Var(0)))), Term(Appl(Term(Var(1)), Term(Var(0))))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "((^ 1) (1 0))");
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
