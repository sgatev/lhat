#include "parser.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace nameless {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(Parser, Var) {
  const std::string expr = "21";
  const Term term = Parser::Parse(expr);

  const auto var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Index(), 21);
}

TEST(Parser, AbstBound) {
  const std::string expr = "(^ 0)";
  const Term term = Parser::Parse(expr);

  const auto abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), -1);
}

TEST(Parser, AbstFree) {
  const std::string expr = "(^ 2)";
  const Term term = Parser::Parse(expr);

  const auto abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), 1);
}

TEST(Parser, MultiAbstBound) {
  const std::string expr = "(^ (^ 1))";
  const Term term = Parser::Parse(expr);

  const auto abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  const auto body_abst = abst->Body().Get<Abst>();
  ASSERT_THAT(body_abst, NotNull());

  const auto body_var = body_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Index(), -1);
}

TEST(Parser, Appl) {
  const std::string expr = "(1 2)";
  const Term term = Parser::Parse(expr);

  const auto appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const auto func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 1);

  const auto arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 2);
}

TEST(Parser, Complex) {
  const std::string expr = "((^ (0 1)) (2 0)";
  const Term term = Parser::Parse(expr);

  const auto appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const auto func_abst = appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const auto body_appl = func_abst->Body().Get<Appl>();
  ASSERT_THAT(body_appl, NotNull());

  const auto body_func_var = body_appl->Func().Get<Var>();
  ASSERT_THAT(body_func_var, NotNull());
  EXPECT_EQ(body_func_var->Index(), -1);

  const auto body_arg_var = body_appl->Arg().Get<Var>();
  ASSERT_THAT(body_arg_var, NotNull());
  EXPECT_EQ(body_arg_var->Index(), 0);

  const auto arg_appl = appl->Arg().Get<Appl>();
  ASSERT_THAT(arg_appl, NotNull());

  const auto arg_func_var = arg_appl->Func().Get<Var>();
  ASSERT_THAT(arg_func_var, NotNull());
  EXPECT_EQ(arg_func_var->Index(), 2);

  const auto arg_arg_var = arg_appl->Arg().Get<Var>();
  ASSERT_THAT(arg_arg_var, NotNull());
  EXPECT_EQ(arg_arg_var->Index(), 0);
}
}  // namespace
}  // namespace nameless
}  // namespace lhat
