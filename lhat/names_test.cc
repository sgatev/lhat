#include "names.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace {
using ::testing::NotNull;

TEST(NameContext, AddAndGetNames) {
  NameContext nctx;

  nctx.AddName("foo");
  nctx.AddName("bar");

  EXPECT_EQ(nctx.GetIndexForName("foo"), 0);
  EXPECT_EQ(nctx.GetNameForIndex(0), "foo");

  EXPECT_EQ(nctx.GetIndexForName("bar"), 1);
  EXPECT_EQ(nctx.GetNameForIndex(1), "bar");
}

TEST(StripNames, Var) {
  const named::Term named_term = named::Var("x");
  const nameless::Term nameless_term = StripNames(named_term);

  const nameless::Var* var = nameless_term.Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);
}

TEST(StripNames, AbstBound) {
  const named::Term named_term = named::Abst("x", named::Var("x"));
  const nameless::Term nameless_term = StripNames(named_term);

  const nameless::Abst* abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const nameless::Var* var = abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), -1);
}

TEST(StripNames, AbstFree) {
  const named::Term named_term = named::Abst("x", named::Var("y"));
  const nameless::Term nameless_term = StripNames(named_term);

  const nameless::Abst* abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const nameless::Var* var = abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);
}

TEST(StripNames, Appl) {
  const named::Term named_term = named::Appl(named::Var("x"), named::Var("y"));
  const nameless::Term nameless_term = StripNames(named_term);

  const nameless::Appl* appl = nameless_term.Get<nameless::Appl>();
  ASSERT_THAT(appl, NotNull());

  const nameless::Var* func_var = appl->Func().Get<nameless::Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 0);

  const nameless::Var* arg_var = appl->Arg().Get<nameless::Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 1);
}

TEST(StripNames, Complex) {
  const named::Term named_term =
      named::Appl(named::Abst("x", named::Var("y")),
                  named::Appl(named::Var("u"), named::Var("v")));
  const nameless::Term nameless_term = StripNames(named_term);

  const nameless::Appl* appl = nameless_term.Get<nameless::Appl>();
  ASSERT_THAT(appl, NotNull());

  const nameless::Abst* func_abst = appl->Func().Get<nameless::Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const nameless::Var* abst_var = func_abst->Body().Get<nameless::Var>();
  ASSERT_THAT(abst_var, NotNull());
  EXPECT_EQ(abst_var->Index(), 0);

  const nameless::Appl* arg_appl = appl->Arg().Get<nameless::Appl>();
  ASSERT_THAT(arg_appl, NotNull());

  const nameless::Var* arg_func_var = arg_appl->Func().Get<nameless::Var>();
  ASSERT_THAT(arg_func_var, NotNull());
  EXPECT_EQ(arg_func_var->Index(), 1);

  const nameless::Var* arg_arg_var = arg_appl->Arg().Get<nameless::Var>();
  ASSERT_THAT(arg_arg_var, NotNull());
  EXPECT_EQ(arg_arg_var->Index(), 2);
}
}  // namespace
}  // namespace lhat
