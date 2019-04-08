#include "reduce.h"

#include "ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::NotNull;

TEST(BetaReduceTerm, Var) {
  Term term = Var(1);
  BetaReduceTerm(&term);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(BetaReduceTerm, Abst) {
  Term term = Abst(Var(1));
  BetaReduceTerm(&term);

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(BetaReduceTerm, ApplNonRedex) {
  Term term = Appl(Var(1), Var(2));
  BetaReduceTerm(&term);

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(BetaReduceTerm, ApplRedex) {
  Term term = Appl(Abst(Var(-1)), Var(2));
  BetaReduceTerm(&term);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 2);
}

TEST(BetaReduceSubTerms, Var) {
  Term term = Var(1);
  BetaReduceSubTerms(&term);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(BetaReduceSubTerms, Abst) {
  Term term = Abst(Var(1));
  BetaReduceSubTerms(&term);

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(BetaReduceSubTerms, ApplNonRedex) {
  Term term = Appl(Var(1), Var(2));
  BetaReduceSubTerms(&term);

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(BetaReduceSubTerms, ApplRedex) {
  Term term = Appl(Abst(Var(-1)), Var(2));
  BetaReduceSubTerms(&term);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 2);
}

TEST(BetaReduceSubTerms, Complex) {
  Term term = Abst(Appl(Abst(Var(-2)), Var(0)));
  BetaReduceSubTerms(&term);

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* var = abst->Body().Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);
}

TEST(IsNormalForm, Var) {
  const Term term = Var(1);
  EXPECT_TRUE(IsNormalForm(term));
}

TEST(IsNormalForm, Abst) {
  const Term term = Abst(Var(1));
  EXPECT_TRUE(IsNormalForm(term));
}

TEST(IsNormalForm, ApplNonRedex) {
  const Term term = Appl(Var(1), Var(2));
  EXPECT_TRUE(IsNormalForm(term));
}

TEST(IsNormalForm, ApplRedex) {
  const Term term = Appl(Abst(Var(-1)), Var(2));
  EXPECT_FALSE(IsNormalForm(term));
}

TEST(IsNormalForm, Complex) {
  const Term term = Abst(Appl(Abst(Var(-1)), Var(2)));
  EXPECT_FALSE(IsNormalForm(term));
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
