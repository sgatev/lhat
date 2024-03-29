#include "lhat/nameless/printer.h"

#include <string>

#include "gtest/gtest.h"

namespace lhat::nameless {
namespace {

TEST(Printer, Var) {
  const Term term = Var(21);
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "21");
}

TEST(Printer, AbstFree) {
  const Term term = Abst(Var(1));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ 2)");
}

TEST(Printer, AbstBound) {
  const Term term = Abst(Var(-1));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ 0)");
}

TEST(Printer, MultiAbstBound) {
  const Term term = Abst(Term(Abst(Var(-1))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ (^ 0))");
}

TEST(Printer, MultiAbstAppl) {
  const Term term = Abst(Term(Abst(Appl(Var(-1), Var(-2)))));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ (^ (0 1)))");
}

TEST(Printer, Appl) {
  const Term term = Appl(Var(1), Var(2));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(1 2)");
}

TEST(Printer, Complex) {
  const Term term = Appl(Abst(Var(0)), Appl(Var(1), Var(0)));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "((^ 1) (1 0))");
}

}  // namespace
}  // namespace lhat::nameless
