#include "parser.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(Parser, Empty) {
  const std::string expr = "";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, IsNull());
}

TEST(Parser, Var) {
  const std::string expr = "21";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(term);
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->idx, 21);
}

TEST(Parser, AbstBound) {
  const std::string expr = "(^ 0)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(term);
  ASSERT_THAT(abst, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, -1);
}

TEST(Parser, AbstFree) {
  const std::string expr = "(^ 2)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(term);
  ASSERT_THAT(abst, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, 1);
}

TEST(Parser, MultiAbstBound) {
  const std::string expr = "(^ (^ 1))";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(term);
  ASSERT_THAT(abst, NotNull());

  const auto body_abst = std::static_pointer_cast<AbstTerm>(abst->body);
  ASSERT_THAT(body_abst, NotNull());

  const auto body_var = std::static_pointer_cast<VarTerm>(body_abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->idx, -1);
}

TEST(Parser, Appl) {
  const std::string expr = "(1 2)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(term);
  ASSERT_THAT(appl, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->idx, 1);

  const auto arg_var = std::static_pointer_cast<VarTerm>(appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->idx, 2);
}

TEST(Parser, Complex) {
  const std::string expr = "((^ (0 1)) (2 0)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(term);
  ASSERT_THAT(appl, NotNull());

  const auto func_abst = std::static_pointer_cast<AbstTerm>(appl->func);
  ASSERT_THAT(func_abst, NotNull());

  const auto body_appl = std::static_pointer_cast<ApplTerm>(func_abst->body);
  ASSERT_THAT(body_appl, NotNull());

  const auto body_func_var = std::static_pointer_cast<VarTerm>(body_appl->func);
  ASSERT_THAT(body_func_var, NotNull());
  EXPECT_EQ(body_func_var->idx, -1);

  const auto body_arg_var = std::static_pointer_cast<VarTerm>(body_appl->arg);
  ASSERT_THAT(body_arg_var, NotNull());
  EXPECT_EQ(body_arg_var->idx, 0);

  const auto arg_appl = std::static_pointer_cast<ApplTerm>(appl->arg);
  ASSERT_THAT(arg_appl, NotNull());

  const auto arg_func_var = std::static_pointer_cast<VarTerm>(arg_appl->func);
  ASSERT_THAT(arg_func_var, NotNull());
  EXPECT_EQ(arg_func_var->idx, 2);

  const auto arg_arg_var = std::static_pointer_cast<VarTerm>(arg_appl->arg);
  ASSERT_THAT(arg_arg_var, NotNull());
  EXPECT_EQ(arg_arg_var->idx, 0);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
