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
  const auto m = VarTerm::Make("x");
  const std::shared_ptr<Term> n = nullptr;
  EXPECT_FALSE(AlphaEquiv(m, n));

  const std::shared_ptr<Term> u = nullptr;
  const auto v = VarTerm::Make("x");
  EXPECT_FALSE(AlphaEquiv(u, v));
}

TEST(AlphaEquiv, DistinctTypes) {
  const auto m = VarTerm::Make("x");
  const auto n = AbstTerm::Make("x", VarTerm::Make("y"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, DistinctFreeVars) {
  const auto m = VarTerm::Make("x");
  const auto n = VarTerm::Make("y");
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, SameFreeVars) {
  const auto m = VarTerm::Make("x");
  const auto n = VarTerm::Make("x");
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstDistinctBoundVars) {
  const auto m = AbstTerm::Make("x", VarTerm::Make("x"));
  const auto n = AbstTerm::Make("y", VarTerm::Make("y"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstSameBoundVars) {
  const auto m = AbstTerm::Make("x", VarTerm::Make("x"));
  const auto n = AbstTerm::Make("x", VarTerm::Make("x"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstDistinctBodies) {
  const auto m = AbstTerm::Make("x", VarTerm::Make("y"));
  const auto n = AbstTerm::Make("x", VarTerm::Make("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplDistinctFuncs) {
  const auto m = ApplTerm::Make(VarTerm::Make("x"), VarTerm::Make("z"));
  const auto n = ApplTerm::Make(VarTerm::Make("y"), VarTerm::Make("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplDistinctArgs) {
  const auto m = ApplTerm::Make(VarTerm::Make("x"), VarTerm::Make("y"));
  const auto n = ApplTerm::Make(VarTerm::Make("x"), VarTerm::Make("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplSameFuncsAndArgs) {
  const auto m = ApplTerm::Make(VarTerm::Make("x"), VarTerm::Make("y"));
  const auto n = ApplTerm::Make(VarTerm::Make("x"), VarTerm::Make("y"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, Complex) {
  const auto m = AbstTerm::Make(
      "u", ApplTerm::Make(AbstTerm::Make("x", VarTerm::Make("x")),
                          VarTerm::Make("y")));
  const auto n = AbstTerm::Make(
      "v", ApplTerm::Make(AbstTerm::Make("z", VarTerm::Make("z")),
                          VarTerm::Make("y")));
  EXPECT_TRUE(AlphaEquiv(m, n));
}
}  // namespace
}  // namespace named
}  // namespace lhat
