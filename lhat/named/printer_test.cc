#include "printer.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(Printer, Null) {
  const std::shared_ptr<Term> term = nullptr;
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "");
}

TEST(Printer, Var) {
  const auto term = VarTerm::Make("x");
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "x");
}

TEST(Printer, Abst) {
  const auto term = AbstTerm::Make("x", VarTerm::Make("y"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ x y)");
}

TEST(Printer, Appl) {
  const auto term = ApplTerm::Make(VarTerm::Make("x"), VarTerm::Make("y"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(x y)");
}

TEST(Printer, Complex) {
  const auto term =
      ApplTerm::Make(AbstTerm::Make("x", VarTerm::Make("y")),
                     ApplTerm::Make(VarTerm::Make("u"), VarTerm::Make("v")));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "((^ x y) (u v))");
}
}  // namespace
}  // namespace named
}  // namespace lhat
