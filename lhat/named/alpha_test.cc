#include "lhat/named/alpha.h"

#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
TEST(IsAlphaEquiv, DistinctTypes) {
  const Term m = Var("x");
  const Term n = Abst("x", Var("y"));
  EXPECT_FALSE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, DistinctFreeVars) {
  const Term m = Var("x");
  const Term n = Var("y");
  EXPECT_FALSE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, SameFreeVars) {
  const Term m = Var("x");
  const Term n = Var("x");
  EXPECT_TRUE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, AbstDistinctBoundVars) {
  const Term m = Abst("x", Var("x"));
  const Term n = Abst("y", Var("y"));
  EXPECT_TRUE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, AbstSameBoundVars) {
  const Term m = Abst("x", Var("x"));
  const Term n = Abst("x", Var("x"));
  EXPECT_TRUE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, AbstDistinctBodies) {
  const Term m = Abst("x", Var("y"));
  const Term n = Abst("x", Var("z"));
  EXPECT_FALSE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, ApplDistinctFuncs) {
  const Term m = Appl(Var("x"), Var("z"));
  const Term n = Appl(Var("y"), Var("z"));
  EXPECT_FALSE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, ApplDistinctArgs) {
  const Term m = Appl(Var("x"), Var("y"));
  const Term n = Appl(Var("x"), Var("z"));
  EXPECT_FALSE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, ApplSameFuncsAndArgs) {
  const Term m = Appl(Var("x"), Var("y"));
  const Term n = Appl(Var("x"), Var("y"));
  EXPECT_TRUE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, Complex) {
  const Term m = Abst("u", Appl(Abst("x", Var("x")), Var("y")));
  const Term n = Abst("v", Appl(Abst("z", Var("z")), Var("y")));
  EXPECT_TRUE(IsAlphaEquiv(m, n));
}

TEST(IsAlphaEquiv, BoundVarIsFreeVarInOtherTerm) {
  const Term m = Abst("y", Appl(Var("y"), Var("y")));
  const Term n = Abst("x", Appl(Var("x"), Var("y")));
  EXPECT_FALSE(IsAlphaEquiv(m, n));
}
}  // namespace
}  // namespace named
}  // namespace lhat
