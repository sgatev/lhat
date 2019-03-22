#include "sub.h"

#include "ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(ShiftFreeVarIndex, Null) {
  const std::shared_ptr<Term> term = nullptr;

  const auto result = ShiftFreeVarIndex(term, 1);

  EXPECT_THAT(result, IsNull());
}

TEST(ShiftFreeVarIndex, Var) {
  const auto term = VarTerm::Make(1);

  const auto result =
      std::static_pointer_cast<VarTerm>(ShiftFreeVarIndex(term, 1));
  ASSERT_THAT(result, NotNull());

  EXPECT_EQ(result->idx, 2);
}

TEST(ShiftFreeVarIndex, Appl) {
  const auto term = ApplTerm::Make(VarTerm::Make(1), VarTerm::Make(2));

  const auto result =
      std::static_pointer_cast<ApplTerm>(ShiftFreeVarIndex(term, 1));
  ASSERT_THAT(result, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(result->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->idx, 2);

  const auto arg_var = std::static_pointer_cast<VarTerm>(result->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->idx, 3);
}

TEST(ShiftFreeVarIndex, AbstFreeVar) {
  const auto term = AbstTerm::Make(VarTerm::Make(1));

  const auto result =
      std::static_pointer_cast<AbstTerm>(ShiftFreeVarIndex(term, 1));
  ASSERT_THAT(result, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(result->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 2);
}

TEST(ShiftFreeVarIndex, AbstBoundVar) {
  const auto term = AbstTerm::Make(VarTerm::Make(0));

  const auto result =
      std::static_pointer_cast<AbstTerm>(ShiftFreeVarIndex(term, 1));
  ASSERT_THAT(result, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(result->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 0);
}

TEST(ShiftFreeVarIndex, Complex) {
  const auto term = AbstTerm::Make(
      AbstTerm::Make(ApplTerm::Make(VarTerm::Make(1), VarTerm::Make(2))));

  const auto result =
      std::static_pointer_cast<AbstTerm>(ShiftFreeVarIndex(term, 1));
  ASSERT_THAT(result, NotNull());

  const auto nested_abst = std::static_pointer_cast<AbstTerm>(result->body);
  ASSERT_THAT(nested_abst, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(nested_abst->body);
  ASSERT_THAT(appl, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->idx, 1);

  const auto arg_var = std::static_pointer_cast<VarTerm>(appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->idx, 3);
}

TEST(Sub, Null) {
  const auto result = Sub(nullptr, 0, VarTerm::Make(1));
  EXPECT_THAT(result, IsNull());
}

TEST(Sub, SameVar) {
  const auto result = Sub(VarTerm::Make(0), 0, VarTerm::Make(1));
  ASSERT_THAT(result, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(result);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->idx, 1);
}

TEST(Sub, OtherVar) {
  const auto result = Sub(VarTerm::Make(1), 0, VarTerm::Make(2));
  ASSERT_THAT(result, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(result);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->idx, 1);
}

TEST(Sub, AbstDistinctVar) {
  const auto result =
      Sub(AbstTerm::Make(VarTerm::Make(0)), 0, VarTerm::Make(1));
  ASSERT_THAT(result, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(result);
  ASSERT_THAT(abst, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 0);
}

TEST(Sub, AbstSameVar) {
  const auto result =
      Sub(AbstTerm::Make(VarTerm::Make(1)), 0, VarTerm::Make(1));
  ASSERT_THAT(result, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(result);
  ASSERT_THAT(abst, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 2);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
