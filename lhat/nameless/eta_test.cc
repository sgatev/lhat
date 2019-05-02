#include "lhat/nameless/eta.h"

#include "lhat/nameless/ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::NotNull;

TEST(EtaReduceTerm, Var) {
  Term term = Var(1);
  EXPECT_FALSE(EtaReduceTerm(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(EtaReduceTerm, Abst) {
  Term term = Abst(Var(1));
  EXPECT_FALSE(EtaReduceTerm(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(EtaReduceTerm, Appl) {
  Term term = Appl(Var(1), Var(2));
  EXPECT_FALSE(EtaReduceTerm(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(EtaReduceTerm, AbstApplFree) {
  Term term = Abst(Appl(Var(1), Var(2)));
  EXPECT_FALSE(EtaReduceTerm(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Appl* body_appl = abst->Body().Get<Appl>();
  ASSERT_THAT(body_appl, NotNull());

  const Var* func_var = body_appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = body_appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(EtaReduceTerm, AbstApplBound) {
  Term term = Abst(Appl(Var(1), Var(-1)));
  EXPECT_TRUE(EtaReduceTerm(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(EtaReduceTerm, AbstApplBoundUsed) {
  Term term = Abst(Appl(Abst(Var(-2)), Var(-1)));
  EXPECT_FALSE(EtaReduceTerm(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Appl* body_appl = abst->Body().Get<Appl>();
  ASSERT_THAT(body_appl, NotNull());

  const Abst* func_abst = body_appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const Var* body_var = func_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), -2);

  const Var* arg_var = body_appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), -1);
}

TEST(EtaReduceTerm, Complex) {
  Term term = Abst(Appl(Abst(Var(1)), Var(-1)));
  EXPECT_TRUE(EtaReduceTerm(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(EtaReduceSubTerms, Var) {
  Term term = Var(1);
  EXPECT_FALSE(EtaReduceSubTerms(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 1);
}

TEST(EtaReduceSubTerms, Abst) {
  Term term = Abst(Var(1));
  EXPECT_FALSE(EtaReduceSubTerms(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(EtaReduceSubTerms, Appl) {
  Term term = Appl(Var(1), Var(2));
  EXPECT_FALSE(EtaReduceSubTerms(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(EtaReduceSubTerms, AbstApplFree) {
  Term term = Abst(Appl(Var(1), Var(2)));
  EXPECT_FALSE(EtaReduceSubTerms(&term));

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const Appl* body_appl = abst->Body().Get<Appl>();
  ASSERT_THAT(body_appl, NotNull());

  const Var* func_var = body_appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const Var* arg_var = body_appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(EtaReduceSubTerms, AbstApplBound) {
  Term term = Abst(Appl(Var(1), Var(-1)));
  EXPECT_TRUE(EtaReduceSubTerms(&term));

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 1);
}

TEST(EtaReduceSubTerms, AbstApplBoundNested) {
  Term term = Appl(Var(2), Abst(Appl(Var(1), Var(-1))));
  EXPECT_TRUE(EtaReduceSubTerms(&term));

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 2);

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 1);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
