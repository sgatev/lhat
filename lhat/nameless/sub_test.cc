#include "lhat/nameless/sub.h"

#include "lhat/nameless/ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::NotNull;

TEST(SubBoundVar, VarReplacement) {
  Term term = Abst(Var(-1));
  const Term replacement = Var(0);
  SubBoundVar(-1, replacement, term.Get<Abst>()->MutableBody());

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 0);
}

TEST(SubBoundVar, NestedVarReplacement) {
  Term term = Abst(Term(Abst(Var(-2))));
  const Term replacement = Var(1);
  SubBoundVar(-1, replacement, term.Get<Abst>()->MutableBody());

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Abst* body_abst = abst->Body().Get<Abst>();
  ASSERT_THAT(body_abst, NotNull());

  const Var* body_var = body_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(SubBoundVar, AbstReplacement) {
  Term term = Abst(Var(-1));
  const Term replacement = Abst(Var(-1));
  SubBoundVar(-1, replacement, term.Get<Abst>()->MutableBody());

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Abst* body_abst = abst->Body().Get<Abst>();
  ASSERT_THAT(body_abst, NotNull());

  const Var* body_var = body_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), -1);
}

TEST(SubBoundVar, NestedApplReplacement) {
  Term term = Abst(Term(Abst(Var(-2))));
  const Term replacement = Appl(Abst(Var(-1)), Var(1));
  SubBoundVar(-1, replacement, term.Get<Abst>()->MutableBody());

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Abst* body_abst = abst->Body().Get<Abst>();
  ASSERT_THAT(body_abst, NotNull());

  const Appl* appl = body_abst->Body().Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Abst* func_abst = appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const Var* abst_var = func_abst->Body().Get<Var>();
  ASSERT_THAT(abst_var, NotNull());
  EXPECT_EQ(abst_var->Index(), -1);

  const Var* var = appl->Arg().Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(SubFreeVar, SameVar) {
  Term term = Var(0);
  const Term replacement = Var(1);
  SubFreeVar(0, replacement, &term);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(SubFreeVar, OtherVar) {
  Term term = Var(1);
  const Term replacement = Var(2);
  SubFreeVar(0, replacement, &term);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(SubFreeVar, AbstDistinctVar) {
  Term term = Abst(Var(2));
  const Term replacement = Var(1);
  SubFreeVar(0, replacement, &term);

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 2);
}

TEST(SubFreeVar, AbstSameVar) {
  Term term = Abst(Var(0));
  const Term replacement = Var(1);
  SubFreeVar(0, replacement, &term);

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(SubFreeVar, AbstApplSameVar) {
  Term term = Appl(Var(0), Abst(Appl(Var(0), Var(1))));
  const Term replacement = Var(1);
  SubFreeVar(0, replacement, &term);

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Abst* abst = appl->Arg().Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Appl* inner_appl = abst->Body().Get<Appl>();
  ASSERT_THAT(inner_appl, NotNull());

  const Var* inner_func_var = inner_appl->Func().Get<Var>();
  ASSERT_THAT(inner_func_var, NotNull());
  EXPECT_EQ(inner_func_var->Index(), 1);

  const Var* inner_arg_var = inner_appl->Arg().Get<Var>();
  ASSERT_THAT(inner_arg_var, NotNull());
  EXPECT_EQ(inner_arg_var->Index(), 1);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
