#include "equiv.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(AlphaEquiv, DistinctTypes) {
  const Term m = Var("x");
  const Term n = Abst("x", Var("y"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, DistinctFreeVars) {
  const Term m = Var("x");
  const Term n = Var("y");
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, SameFreeVars) {
  const Term m = Var("x");
  const Term n = Var("x");
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstDistinctBoundVars) {
  const Term m = Abst("x", Var("x"));
  const Term n = Abst("y", Var("y"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstSameBoundVars) {
  const Term m = Abst("x", Var("x"));
  const Term n = Abst("x", Var("x"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, AbstDistinctBodies) {
  const Term m = Abst("x", Var("y"));
  const Term n = Abst("x", Var("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplDistinctFuncs) {
  const Term m = Appl(Var("x"), Var("z"));
  const Term n = Appl(Var("y"), Var("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplDistinctArgs) {
  const Term m = Appl(Var("x"), Var("y"));
  const Term n = Appl(Var("x"), Var("z"));
  EXPECT_FALSE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, ApplSameFuncsAndArgs) {
  const Term m = Appl(Var("x"), Var("y"));
  const Term n = Appl(Var("x"), Var("y"));
  EXPECT_TRUE(AlphaEquiv(m, n));
}

TEST(AlphaEquiv, Complex) {
  const Term m = Abst("u", Appl(Abst("x", Var("x")), Var("y")));
  const Term n = Abst("v", Appl(Abst("z", Var("z")), Var("y")));
  EXPECT_TRUE(AlphaEquiv(m, n));
}
}  // namespace
}  // namespace named
}  // namespace lhat
