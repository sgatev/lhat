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
  const auto term = std::make_shared<VarTerm>("x");
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "x");
}

TEST(Printer, Abst) {
  const auto term =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(^ x y)");
}

TEST(Printer, Appl) {
  const auto term = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                               std::make_shared<VarTerm>("y"));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "(x y)");
}

TEST(Printer, Complex) {
  const auto term = std::make_shared<ApplTerm>(
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")),
      std::make_shared<ApplTerm>(std::make_shared<VarTerm>("u"),
                                 std::make_shared<VarTerm>("v")));
  std::string out;
  Printer::Print(term, &out);
  EXPECT_EQ(out, "((^ x y) (u v))");
}
}  // namespace
}  // namespace named
}  // namespace lhat
