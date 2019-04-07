#include "sub.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(Sub, SameVar) {
  const Term target(Var("x"));
  const Term replacement(Var("y"));
  const Term result = Sub(target, "x", replacement);

  const Var* var = result.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "y");
}

TEST(Sub, OtherVar) {
  const Term target(Var("z"));
  const Term replacement(Var("y"));
  const Term result = Sub(target, "x", replacement);

  const Var* var = result.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "z");
}

TEST(Sub, AbstSameVar) {
  const Term target(Abst("x", Term(Var("x"))));
  const Term replacement(Var("y"));
  const Term result = Sub(target, "x", replacement);

  const Abst* abst = result.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "x");
}

TEST(Sub, AbstOtherVar) {
  const Term target(Abst("x", Term(Var("y"))));
  const Term replacement(Var("z"));
  const Term result = Sub(target, "y", replacement);

  const Abst* abst = result.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "z");
}

TEST(Sub, Appl) {
  const Term target(Appl(Term(Var("x")), Term(Var("x"))));
  const Term replacement(Var("y"));
  const Term result = Sub(target, "x", replacement);

  const Appl* appl = result.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "y");

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "y");
}

TEST(SafeSub, SameVar) {
  const Term target(Var("x"));
  const Term replacement(Var("y"));
  const Term result = SafeSub(target, "x", replacement);

  const Var* var = result.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "y");
}

TEST(SafeSub, OtherVar) {
  const Term target(Var("z"));
  const Term replacement(Var("y"));
  const Term result = SafeSub(target, "x", replacement);

  const Var* var = result.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "z");
}

TEST(SafeSub, AbstSameVar) {
  const Term target(Abst("x", Term(Var("x"))));
  const Term replacement(Var("y"));
  const Term result = SafeSub(target, "x", replacement);

  const Abst* abst = result.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "x");
}

TEST(SafeSub, AbstOtherVar) {
  const Term target(Abst("x", Term(Var("y"))));
  const Term replacement(Var("z"));
  const Term result = SafeSub(target, "y", replacement);

  const Abst* abst = result.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "z");
}

TEST(SafeSub, Appl) {
  const Term target(Appl(Term(Var("x")), Term(Var("x"))));
  const Term replacement(Var("y"));
  const Term result = SafeSub(target, "x", replacement);

  const Appl* appl = result.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "y");

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "y");
}
}  // namespace
}  // namespace named
}  // namespace lhat
