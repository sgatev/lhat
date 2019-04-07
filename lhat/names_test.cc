#include "names.h"

#include <memory>

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
  const auto named_term = named::VarTerm::Make("x");
  const nameless::Term nameless_term = StripNames(named_term);

  const auto var = nameless_term.Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);
}

TEST(StripNames, AbstBound) {
  const auto named_term = named::AbstTerm::Make("x", named::VarTerm::Make("x"));
  const nameless::Term nameless_term = StripNames(named_term);

  const auto abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto var = abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), -1);
}

TEST(StripNames, AbstFree) {
  const auto named_term = named::AbstTerm::Make("x", named::VarTerm::Make("y"));
  const nameless::Term nameless_term = StripNames(named_term);

  const auto abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto var = abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);
}

TEST(StripNames, Appl) {
  const auto named_term = named::ApplTerm::Make(named::VarTerm::Make("x"),
                                                named::VarTerm::Make("y"));
  const nameless::Term nameless_term = StripNames(named_term);

  const auto appl = nameless_term.Get<nameless::Appl>();
  ASSERT_THAT(appl, NotNull());

  const auto func_var = appl->Func().Get<nameless::Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 0);

  const auto arg_var = appl->Arg().Get<nameless::Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 1);
}

TEST(StripNames, Complex) {
  const auto named_term = named::ApplTerm::Make(
      named::AbstTerm::Make("x", named::VarTerm::Make("y")),
      named::ApplTerm::Make(named::VarTerm::Make("u"),
                            named::VarTerm::Make("v")));
  const nameless::Term nameless_term = StripNames(named_term);

  const auto appl = nameless_term.Get<nameless::Appl>();
  ASSERT_THAT(appl, NotNull());

  const auto func_abst = appl->Func().Get<nameless::Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const auto abst_var = func_abst->Body().Get<nameless::Var>();
  ASSERT_THAT(abst_var, NotNull());
  EXPECT_EQ(abst_var->Index(), 0);

  const auto arg_appl = appl->Arg().Get<nameless::Appl>();
  ASSERT_THAT(arg_appl, NotNull());

  const auto arg_func_var = arg_appl->Func().Get<nameless::Var>();
  ASSERT_THAT(arg_func_var, NotNull());
  EXPECT_EQ(arg_func_var->Index(), 1);

  const auto arg_arg_var = arg_appl->Arg().Get<nameless::Var>();
  ASSERT_THAT(arg_arg_var, NotNull());
  EXPECT_EQ(arg_arg_var->Index(), 2);
}
}  // namespace
}  // namespace lhat
