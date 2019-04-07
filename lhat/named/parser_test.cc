#include "parser.h"

#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(Parser, Var) {
  const std::string expr = "x";
  const Term term = Parser::Parse(expr);

  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Name(), "x");
}

TEST(Parser, Abst) {
  const std::string expr = "(^ x y)";
  const Term term = Parser::Parse(expr);

  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "y");
}

TEST(Parser, Appl) {
  const std::string expr = "(x y)";
  const Term term = Parser::Parse(expr);

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "x");

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "y");
}

TEST(Parser, Complex) {
  const std::string expr = "((^ x y) (u v)";
  const Term term = Parser::Parse(expr);

  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Abst* func_abst = appl->Func().Get<Abst>();
  ASSERT_THAT(func_abst, NotNull());

  EXPECT_EQ(func_abst->VarName(), "x");

  const Var* body_var = func_abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "y");

  const Appl* arg_appl = appl->Arg().Get<Appl>();
  ASSERT_THAT(arg_appl, NotNull());

  const Var* func_var = arg_appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "u");

  const Var* arg_var = arg_appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "v");
}
}  // namespace
}  // namespace named
}  // namespace lhat
