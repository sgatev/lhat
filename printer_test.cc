#include "printer.h"

#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace {
TEST(Printer, Null) {
  const std::shared_ptr<Term> term = nullptr;
  EXPECT_EQ(Printer::Print(term), "");
}

TEST(Printer, Var) {
  const auto term = std::make_shared<VarTerm>("x");
  EXPECT_EQ(Printer::Print(term), "x");
}

TEST(Printer, Abst) {
  const auto term =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));
  EXPECT_EQ(Printer::Print(term), "(^ x y)");
}

TEST(Printer, Appl) {
  const auto term = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                               std::make_shared<VarTerm>("y"));
  EXPECT_EQ(Printer::Print(term), "(x y)");
}

TEST(Printer, Complex) {
  const auto term = std::make_shared<ApplTerm>(
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")),
      std::make_shared<ApplTerm>(std::make_shared<VarTerm>("u"),
                                 std::make_shared<VarTerm>("v")));
  EXPECT_EQ(Printer::Print(term), "((^ x y) (u v))");
}
}  // namespace
}  // namespace lhat
