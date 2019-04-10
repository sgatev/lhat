#include "lhat/named/sub.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
using ::testing::NotNull;

TEST(Sub, SameVar) {
  Term target = Var("x");
  const Term replacement = Var("y");
  Sub("x", replacement, &target);

  const Var* var = target.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "y");
}

TEST(Sub, OtherVar) {
  Term target = Var("z");
  const Term replacement = Var("y");
  Sub("x", replacement, &target);

  const Var* var = target.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "z");
}

TEST(Sub, AbstSameVar) {
  Term target = Abst("x", Var("x"));
  const Term replacement = Var("y");
  Sub("x", replacement, &target);

  const Abst* abst = target.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "x");
}

TEST(Sub, AbstOtherVar) {
  Term target = Abst("x", Var("y"));
  const Term replacement = Var("z");
  Sub("y", replacement, &target);

  const Abst* abst = target.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "z");
}

TEST(Sub, Appl) {
  Term target = Appl(Var("x"), Var("x"));
  const Term replacement = Var("y");
  Sub("x", replacement, &target);

  const Appl* appl = target.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "y");

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "y");
}

TEST(SafeSub, SameVar) {
  Term target = Var("x");
  const Term replacement = Var("y");
  SafeSub("x", replacement, &target);

  const Var* var = target.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "y");
}

TEST(SafeSub, OtherVar) {
  Term target = Var("z");
  const Term replacement = Var("y");
  SafeSub("x", replacement, &target);

  const Var* var = target.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "z");
}

TEST(SafeSub, AbstSameVar) {
  Term target = Abst("x", Var("x"));
  const Term replacement = Var("y");
  SafeSub("x", replacement, &target);

  const Abst* abst = target.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "x");
}

TEST(SafeSub, AbstOtherVar) {
  Term target = Abst("x", Var("y"));
  const Term replacement = Var("z");
  SafeSub("y", replacement, &target);

  const Abst* abst = target.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "z");
}

TEST(SafeSub, Appl) {
  Term target = Appl(Var("x"), Var("x"));
  const Term replacement = Var("y");
  SafeSub("x", replacement, &target);

  const Appl* appl = target.Get<Appl>();
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
