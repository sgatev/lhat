#include "lhat/nameless/reduce.h"

#include "lhat/nameless/ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::NotNull;

TEST(BetaReduceTerm, Var) {
  Term term = Var(1);
  EXPECT_FALSE(BetaReduceTerm(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(BetaReduceTerm, Abst) {
  Term term = Abst(Var(1));
  EXPECT_FALSE(BetaReduceTerm(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(BetaReduceTerm, ApplNonRedex) {
  Term term = Appl(Var(1), Var(2));
  EXPECT_FALSE(BetaReduceTerm(&term));

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
  EXPECT_TRUE(BetaReduceTerm(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 2);
}

TEST(BetaReduceAppl, Var) {
  Term term = Var(1);
  EXPECT_FALSE(BetaReduceAppl(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(BetaReduceAppl, Abst) {
  Term term = Abst(Var(1));
  EXPECT_FALSE(BetaReduceAppl(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(BetaReduceAppl, ApplNonRedex) {
  Term term = Appl(Var(1), Var(2));
  EXPECT_FALSE(BetaReduceAppl(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(BetaReduceAppl, ApplRedex) {
  Term term = Appl(Abst(Var(-1)), Var(2));
  EXPECT_TRUE(BetaReduceAppl(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 2);
}

TEST(BetaReduceAppl, BoundVarOutOfScope) {
  Term term = Abst(Appl(Abst(Var(-2)), Var(0)));
  EXPECT_TRUE(BetaReduceAppl(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* var = abst->Body().Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), -2);
}

TEST(BetaReduceAppl, BoundVarNestedAbst) {
  Term term = Appl(Abst(Term(Abst(Var(-2)))), Var(0));
  EXPECT_TRUE(BetaReduceAppl(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* var = abst->Body().Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(BetaReduceAppl, FuncBeforeArg) {
  Term term = Appl(Abst(Appl(Abst(Var(-1)), Var(0))),
                   Abst(Appl(Abst(Var(-1)), Var(1))));
  EXPECT_TRUE(BetaReduceAppl(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Abst* func_abst = appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const Var* body_var = func_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 0);

  const Abst* arg_abst = appl->Arg().Get<Abst>();
  ASSERT_THAT(arg_abst, NotNull());

  const Appl* body_appl = arg_abst->Body().Get<Appl>();
  ASSERT_THAT(body_appl, NotNull());

  const Abst* body_func_abst = body_appl->Func().Get<Abst>();
  ASSERT_THAT(body_func_abst, NotNull());

  const Var* abst_body_var = body_func_abst->Body().Get<Var>();
  ASSERT_THAT(abst_body_var, NotNull());
  EXPECT_EQ(abst_body_var->Index(), -1);

  const Var* body_arg_var = body_appl->Arg().Get<Var>();
  ASSERT_THAT(body_arg_var, NotNull());
  EXPECT_EQ(body_arg_var->Index(), 1);
}

TEST(BetaReduceAppl, ArgBeforeAppl) {
  Term term = Appl(Abst(Var(0)), Abst(Appl(Abst(Var(-1)), Var(1))));
  EXPECT_TRUE(BetaReduceAppl(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Abst* func_abst = appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const Var* func_body_var = func_abst->Body().Get<Var>();
  ASSERT_THAT(func_body_var, NotNull());
  EXPECT_EQ(func_body_var->Index(), 0);

  const Abst* arg_abst = appl->Arg().Get<Abst>();
  ASSERT_THAT(arg_abst, NotNull());

  const Var* arg_body_var = arg_abst->Body().Get<Var>();
  ASSERT_THAT(arg_body_var, NotNull());
  EXPECT_EQ(arg_body_var->Index(), 1);
}

TEST(IsBetaRedex, Var) {
  const Term term = Var(1);
  EXPECT_FALSE(IsBetaRedex(term));
}

TEST(IsBetaRedex, Abst) {
  const Term term = Abst(Var(1));
  EXPECT_FALSE(IsBetaRedex(term));
}

TEST(IsBetaRedex, ApplNonRedex) {
  const Term term = Appl(Var(1), Var(2));
  EXPECT_FALSE(IsBetaRedex(term));
}

TEST(IsBetaRedex, ApplRedex) {
  const Term term = Appl(Abst(Var(-1)), Var(2));
  EXPECT_TRUE(IsBetaRedex(term));
}

TEST(IsBetaRedex, NestedRedex) {
  const Term term = Abst(Appl(Abst(Var(-1)), Var(2)));
  EXPECT_FALSE(IsBetaRedex(term));
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

TEST(IsNormalForm, NestedRedex) {
  const Term term = Abst(Appl(Abst(Var(-1)), Var(2)));
  EXPECT_FALSE(IsNormalForm(term));
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
