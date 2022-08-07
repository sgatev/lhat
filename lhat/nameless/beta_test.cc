#include "lhat/nameless/beta.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "lhat/nameless/ast.h"

namespace lhat::nameless {
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
  EXPECT_EQ(var->Index(), 0);
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

TEST(BetaReduceNormal, Var) {
  Term term = Var(1);
  EXPECT_FALSE(BetaReduceNormal(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(BetaReduceNormal, Abst) {
  Term term = Abst(Var(1));
  EXPECT_FALSE(BetaReduceNormal(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(BetaReduceNormal, ApplNonRedex) {
  Term term = Appl(Var(1), Var(2));
  EXPECT_FALSE(BetaReduceNormal(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(BetaReduceNormal, ApplRedex) {
  Term term = Appl(Abst(Var(-1)), Var(2));
  EXPECT_TRUE(BetaReduceNormal(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 2);
}

TEST(BetaReduceNormal, BoundVarOutOfScope) {
  Term term = Abst(Appl(Abst(Var(-2)), Var(0)));
  EXPECT_TRUE(BetaReduceNormal(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* var = abst->Body().Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), -2);
}

TEST(BetaReduceNormal, BoundVarNestedAbst) {
  Term term = Appl(Abst(Term(Abst(Var(-2)))), Var(0));
  EXPECT_TRUE(BetaReduceNormal(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* var = abst->Body().Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);
}

TEST(BetaReduceNormal, ApplBeforeFunc) {
  Term term = Appl(Abst(Appl(Abst(Var(-1)), Var(-1))), Var(1));
  EXPECT_TRUE(BetaReduceNormal(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Abst* func_abst = appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const Var* abst_body = func_abst->Body().Get<Var>();
  ASSERT_THAT(abst_body, NotNull());
  EXPECT_EQ(abst_body->Index(), -1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 1);
}

TEST(BetaReduceNormal, FuncBeforeArg) {
  Term term = Appl(Appl(Abst(Var(-1)), Var(0)), Appl(Abst(Var(-1)), Var(1)));
  EXPECT_TRUE(BetaReduceNormal(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 0);

  const Appl* arg_appl = appl->Arg().Get<Appl>();
  ASSERT_THAT(arg_appl, NotNull());

  const Abst* func_abst = arg_appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const Var* body_var = func_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), -1);

  const Var* arg_var = arg_appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 1);
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

TEST(IsBetaNormalForm, Var) {
  const Term term = Var(1);
  EXPECT_TRUE(IsBetaNormalForm(term));
}

TEST(IsBetaNormalForm, Abst) {
  const Term term = Abst(Var(1));
  EXPECT_TRUE(IsBetaNormalForm(term));
}

TEST(IsBetaNormalForm, ApplNonRedex) {
  const Term term = Appl(Var(1), Var(2));
  EXPECT_TRUE(IsBetaNormalForm(term));
}

TEST(IsBetaNormalForm, ApplRedex) {
  const Term term = Appl(Abst(Var(-1)), Var(2));
  EXPECT_FALSE(IsBetaNormalForm(term));
}

TEST(IsBetaNormalForm, NestedRedex) {
  const Term term = Abst(Appl(Abst(Var(-1)), Var(2)));
  EXPECT_FALSE(IsBetaNormalForm(term));
}

TEST(IsHeadNormalForm, Var) {
  const Term term = Var(1);
  EXPECT_TRUE(IsHeadNormalForm(term));
}

TEST(IsHeadNormalForm, Abst) {
  const Term term = Abst(Var(1));
  EXPECT_TRUE(IsHeadNormalForm(term));
}

TEST(IsHeadNormalForm, ApplNonRedex) {
  const Term term = Appl(Var(1), Var(2));
  EXPECT_TRUE(IsHeadNormalForm(term));
}

TEST(IsHeadNormalForm, HeadRedex) {
  const Term term = Appl(Abst(Var(-1)), Var(2));
  EXPECT_FALSE(IsHeadNormalForm(term));
}

TEST(IsHeadNormalForm, NestedHeadRedex) {
  const Term term = Abst(Abst(Appl(Abst(Var(-1)), Var(2))));
  EXPECT_FALSE(IsHeadNormalForm(term));
}

TEST(IsHeadNormalForm, NonHeadRedex) {
  const Term term = Abst(Abst(Appl(Var(3), Appl(Abst(Var(-1)), Var(2)))));
  EXPECT_TRUE(IsHeadNormalForm(term));
}

}  // namespace
}  // namespace lhat::nameless
