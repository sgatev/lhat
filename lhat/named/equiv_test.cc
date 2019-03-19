#include "equiv.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(AlphaEquiv, Null) {
  const std::shared_ptr<Term> m = nullptr;
  const std::shared_ptr<Term> n = nullptr;
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, OneIsNull) {
  const auto m = std::make_shared<VarTerm>("x");
  const std::shared_ptr<Term> n = nullptr;
  EXPECT_FALSE(AlphaEquiv(m, n));

  const std::shared_ptr<Term> u = nullptr;
  const auto v = std::make_shared<VarTerm>("x");
  EXPECT_FALSE(AlphaEquiv(u, v));
}

TEST(AlphaEquiv, DistinctTypes) {
  const auto m = std::make_shared<VarTerm>("x");
  const auto n =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, DistinctFreeVars) {
  const auto m = std::make_shared<VarTerm>("x");
  const auto n = std::make_shared<VarTerm>("y");
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, SameFreeVars) {
  const auto m = std::make_shared<VarTerm>("x");
  const auto n = std::make_shared<VarTerm>("x");
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstDistinctBoundVars) {
  const auto m =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("x"));
  const auto n =
      std::make_shared<AbstTerm>("y", std::make_shared<VarTerm>("y"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstSameBoundVars) {
  const auto m =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("x"));
  const auto n =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("x"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstDistinctBodies) {
  const auto m =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));
  const auto n =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplDistinctFuncs) {
  const auto m = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                            std::make_shared<VarTerm>("z"));
  const auto n = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("y"),
                                            std::make_shared<VarTerm>("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplDistinctArgs) {
  const auto m = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                            std::make_shared<VarTerm>("y"));
  const auto n = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                            std::make_shared<VarTerm>("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplSameFuncsAndArgs) {
  const auto m = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                            std::make_shared<VarTerm>("y"));
  const auto n = std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                            std::make_shared<VarTerm>("y"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, Complex) {
  const auto m = std::make_shared<AbstTerm>(
      "u", std::make_shared<ApplTerm>(
               std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("x")),
               std::make_shared<VarTerm>("y")));
  const auto n = std::make_shared<AbstTerm>(
      "v", std::make_shared<ApplTerm>(
               std::make_shared<AbstTerm>("z", std::make_shared<VarTerm>("z")),
               std::make_shared<VarTerm>("y")));
  EXPECT_TRUE(AlphaEquiv(m, n));
}
}  // namespace
}  // namespace named
}  // namespace lhat
