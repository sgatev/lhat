#include "printer.h"

#include <memory>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
TEST(Printer, Null) {
  const std::shared_ptr<Term> term = nullptr;
  EXPECT_EQ(Printer::Print(term), "");
}

TEST(Printer, Var) {
  const auto term = std::make_shared<VarTerm>(21);
  EXPECT_EQ(Printer::Print(term), "21");
}

TEST(Printer, AbstFree) {
  const auto term = std::make_shared<AbstTerm>(std::make_shared<VarTerm>(1));
  EXPECT_EQ(Printer::Print(term), "(^ 2)");
}

TEST(Printer, AbstBound) {
  const auto term = std::make_shared<AbstTerm>(std::make_shared<VarTerm>(-1));
  EXPECT_EQ(Printer::Print(term), "(^ 0)");
}

TEST(Printer, MultiAbstBound) {
  const auto term = std::make_shared<AbstTerm>(
      std::make_shared<AbstTerm>(std::make_shared<VarTerm>(-1)));
  EXPECT_EQ(Printer::Print(term), "(^ (^ 1))");
}

TEST(Printer, Appl) {
  const auto term = std::make_shared<ApplTerm>(std::make_shared<VarTerm>(1),
                                               std::make_shared<VarTerm>(2));
  EXPECT_EQ(Printer::Print(term), "(1 2)");
}

TEST(Printer, Complex) {
  const auto term = std::make_shared<ApplTerm>(
      std::make_shared<AbstTerm>(std::make_shared<VarTerm>(0)),
      std::make_shared<ApplTerm>(std::make_shared<VarTerm>(1),
                                 std::make_shared<VarTerm>(0)));
  EXPECT_EQ(Printer::Print(term), "((^ 1) (1 0))");
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
