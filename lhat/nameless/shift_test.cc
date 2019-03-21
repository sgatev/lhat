#include "shift.h"

#include "ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(ShiftFreeVarIndex, Null) {
  std::shared_ptr<Term> term = nullptr;

  ShiftFreeVarIndex(1, term);

  EXPECT_THAT(term, IsNull());
}

TEST(ShiftFreeVarIndex, Var) {
  auto term = VarTerm::Make(1);

  ShiftFreeVarIndex(1, term);

  EXPECT_EQ(term->idx, 2);
}

TEST(ShiftFreeVarIndex, Appl) {
  auto term = ApplTerm::Make(VarTerm::Make(1), VarTerm::Make(2));

  ShiftFreeVarIndex(1, term);

  const auto func_var = std::static_pointer_cast<VarTerm>(term->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->idx, 2);

  const auto arg_var = std::static_pointer_cast<VarTerm>(term->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->idx, 3);
}

TEST(ShiftFreeVarIndex, AbstFreeVar) {
  auto term = AbstTerm::Make(VarTerm::Make(1));

  ShiftFreeVarIndex(1, term);

  const auto body_var = std::static_pointer_cast<VarTerm>(term->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 2);
}

TEST(ShiftFreeVarIndex, AbstBoundVar) {
  auto term = AbstTerm::Make(VarTerm::Make(0));

  ShiftFreeVarIndex(1, term);

  const auto body_var = std::static_pointer_cast<VarTerm>(term->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 0);
}

TEST(ShiftFreeVarIndex, Complex) {
  auto term = AbstTerm::Make(
      AbstTerm::Make(ApplTerm::Make(VarTerm::Make(1), VarTerm::Make(2))));

  ShiftFreeVarIndex(1, term);

  const auto nested_abst = std::static_pointer_cast<AbstTerm>(term->body);
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
}  // namespace
}  // namespace nameless
}  // namespace lhat
