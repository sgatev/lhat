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

  const std::shared_ptr<nameless::Term> nameless_term = StripNames(named_term);
  ASSERT_THAT(nameless_term, NotNull());

  const auto var = std::static_pointer_cast<nameless::VarTerm>(nameless_term);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->idx, 0);
}

TEST(StripNames, AbstBound) {
  const auto named_term = named::AbstTerm::Make("x", named::VarTerm::Make("x"));

  const std::shared_ptr<nameless::Term> nameless_term = StripNames(named_term);
  ASSERT_THAT(nameless_term, NotNull());

  const auto abst = std::static_pointer_cast<nameless::AbstTerm>(nameless_term);
  ASSERT_THAT(abst, NotNull());

  const auto var = std::static_pointer_cast<nameless::VarTerm>(abst->body);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->idx, -1);
}

TEST(StripNames, AbstFree) {
  const auto named_term = named::AbstTerm::Make("x", named::VarTerm::Make("y"));

  const std::shared_ptr<nameless::Term> nameless_term = StripNames(named_term);
  ASSERT_THAT(nameless_term, NotNull());

  const auto abst = std::static_pointer_cast<nameless::AbstTerm>(nameless_term);
  ASSERT_THAT(abst, NotNull());

  const auto var = std::static_pointer_cast<nameless::VarTerm>(abst->body);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->idx, 0);
}

TEST(StripNames, Appl) {
  const auto named_term = named::ApplTerm::Make(named::VarTerm::Make("x"),
                                                named::VarTerm::Make("y"));

  const std::shared_ptr<nameless::Term> nameless_term = StripNames(named_term);
  ASSERT_THAT(nameless_term, NotNull());

  const auto appl = std::static_pointer_cast<nameless::ApplTerm>(nameless_term);
  ASSERT_THAT(appl, NotNull());

  const auto func_var = std::static_pointer_cast<nameless::VarTerm>(appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->idx, 0);

  const auto arg_var = std::static_pointer_cast<nameless::VarTerm>(appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->idx, 1);
}

TEST(StripNames, Complex) {
  const auto named_term = named::ApplTerm::Make(
      named::AbstTerm::Make("x", named::VarTerm::Make("y")),
      named::ApplTerm::Make(named::VarTerm::Make("u"),
                            named::VarTerm::Make("v")));

  const std::shared_ptr<nameless::Term> nameless_term = StripNames(named_term);
  ASSERT_THAT(nameless_term, NotNull());

  const auto appl = std::static_pointer_cast<nameless::ApplTerm>(nameless_term);
  ASSERT_THAT(appl, NotNull());

  const auto func_abst =
      std::static_pointer_cast<nameless::AbstTerm>(appl->func);
  ASSERT_THAT(func_abst, NotNull());

  const auto abst_var =
      std::static_pointer_cast<nameless::VarTerm>(func_abst->body);
  ASSERT_THAT(abst_var, NotNull());
  EXPECT_EQ(abst_var->idx, 0);

  const auto arg_appl = std::static_pointer_cast<nameless::ApplTerm>(appl->arg);
  ASSERT_THAT(arg_appl, NotNull());

  const auto arg_func_var =
      std::static_pointer_cast<nameless::VarTerm>(arg_appl->func);
  ASSERT_THAT(arg_func_var, NotNull());
  EXPECT_EQ(arg_func_var->idx, 1);

  const auto arg_arg_var =
      std::static_pointer_cast<nameless::VarTerm>(arg_appl->arg);
  ASSERT_THAT(arg_arg_var, NotNull());
  EXPECT_EQ(arg_arg_var->idx, 2);
}
}  // namespace
}  // namespace lhat
