#include "parser.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(Parser, Empty) {
  const std::string expr = "";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, IsNull());
}

TEST(Parser, Var) {
  const std::string expr = "x";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(term);
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->name, "x");
}

TEST(Parser, Abst) {
  const std::string expr = "(^ x y)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(term);
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->var_name, "x");

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->name, "y");
}

TEST(Parser, Appl) {
  const std::string expr = "(x y)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(term);
  ASSERT_THAT(appl, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->name, "x");

  const auto arg_var = std::static_pointer_cast<VarTerm>(appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->name, "y");
}

TEST(Parser, Complex) {
  const std::string expr = "((^ x y) (u v)";

  const std::shared_ptr<Term> term = Parser::Parse(expr);
  ASSERT_THAT(term, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(term);
  ASSERT_THAT(appl, NotNull());

  const auto func_abst = std::static_pointer_cast<AbstTerm>(appl->func);
  ASSERT_THAT(func_abst, NotNull());

  EXPECT_EQ(func_abst->var_name, "x");

  const auto body_var = std::static_pointer_cast<VarTerm>(func_abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->name, "y");

  const auto arg_appl = std::static_pointer_cast<ApplTerm>(appl->arg);
  ASSERT_THAT(arg_appl, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(arg_appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->name, "u");

  const auto arg_var = std::static_pointer_cast<VarTerm>(arg_appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->name, "v");
}
}  // namespace
}  // namespace lhat
