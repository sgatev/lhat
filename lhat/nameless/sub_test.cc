#include "sub.h"

#include "ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(ShiftFreeVarIndex, Var) {
  const Term term(Var(1));

  const Term shifted = ShiftFreeVarIndex(term, 1);
  const auto result = shifted.Get<Var>();
  ASSERT_THAT(result, NotNull());

  EXPECT_EQ(result->Index(), 2);
}

TEST(ShiftFreeVarIndex, Appl) {
  const Term term(Appl(Term(Var(1)), Term(Var(2))));

  const Term shifted = ShiftFreeVarIndex(term, 1);
  const auto result = shifted.Get<Appl>();
  ASSERT_THAT(result, NotNull());

  const auto func_var = result->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 2);

  const auto arg_var = result->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 3);
}

TEST(ShiftFreeVarIndex, AbstFreeVar) {
  const Term term(Abst(Term(Var(1))));

  const Term shifted = ShiftFreeVarIndex(term, 1);
  const auto result = shifted.Get<Abst>();
  ASSERT_THAT(result, NotNull());

  const auto body_var = result->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 2);
}

TEST(ShiftFreeVarIndex, AbstBoundVar) {
  const Term term(Abst(Term(Var(0))));

  const Term shifted = ShiftFreeVarIndex(term, 1);
  const auto result = shifted.Get<Abst>();
  ASSERT_THAT(result, NotNull());

  const auto body_var = result->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 0);
}

TEST(ShiftFreeVarIndex, Complex) {
  const Term term(Abst(Term(Abst(Term(Appl(Term(Var(1)), Term(Var(2))))))));

  const Term shifted = ShiftFreeVarIndex(term, 1);
  const auto result = shifted.Get<Abst>();
  ASSERT_THAT(result, NotNull());

  const auto nested_abst = result->Body().Get<Abst>();
  ASSERT_THAT(nested_abst, NotNull());

  const auto appl = nested_abst->Body().Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const auto func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const auto arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 3);
}

TEST(Sub, SameVar) {
  const Term target(Var(0));
  const auto result = Sub(target, 0, Term(Var(1)));

  const auto var = result.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(Sub, OtherVar) {
  const Term target(Var(1));
  const auto result = Sub(target, 0, Term(Var(2)));

  const auto var = result.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(Sub, AbstDistinctVar) {
  const Term target(Abst(Term(Var(0))));
  const auto result = Sub(target, 0, Term(Var(1)));

  const auto abst = result.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 0);
}

TEST(Sub, AbstSameVar) {
  const Term target(Abst(Term(Var(1))));
  const auto result = Sub(target, 0, Term(Var(1)));

  const auto abst = result.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 2);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
