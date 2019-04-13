#include "lhat/named/parse.h"

#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
namespace {
using ::testing::NotNull;

TEST(Parse, Var) {
  const std::string expr = "x";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_TRUE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), expr.size());

  const Term term = parse_result.Value();
  const Var* var = term.Get<Var>();
  ASSERT_THAT(var, NotNull());

  EXPECT_EQ(var->Name(), "x");
}

TEST(Parse, Abst) {
  const std::string expr = "(^ x y)";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_TRUE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), expr.size());

  const Term term = parse_result.Value();
  const Abst* abst = term.Get<Abst>();
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->VarName(), "x");

  const Var* body_var = abst->Body().Get<Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "y");
}

TEST(Parse, Appl) {
  const std::string expr = "(x y)";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_TRUE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), expr.size());

  const Term term = parse_result.Value();
  const Appl* appl = term.Get<Appl>();
  ASSERT_THAT(appl, NotNull());

  const Var* func_var = appl->Func().Get<Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "x");

  const Var* arg_var = appl->Arg().Get<Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "y");
}

TEST(Parse, Complex) {
  const std::string expr = "((^ x y) (u v))";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_TRUE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), expr.size());

  const Term term = Parse(expr).Value();
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

TEST(Parse, Whitespace) {
  const std::string expr = "(  ( ^   x  y  )    (  u   v )  )";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_TRUE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), expr.size());

  const Term term = Parse(expr).Value();
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

TEST(Parse, EmptyExpr) {
  const std::string expr = "";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_FALSE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), 0);
  EXPECT_EQ(parse_result.Error().Message(),
            "Failed to parse term: given expression is empty");
}

TEST(Parse, IncompleteExpr) {
  const std::string expr = "(^ (";
  const core::ParseResult<Term> parse_result = Parse(expr);
  EXPECT_FALSE(parse_result.Ok());
  EXPECT_EQ(parse_result.ConsumedChars(), 4);
  EXPECT_EQ(parse_result.Error().Message(),
            "Failed to parse term: ( is not closed");
}
}  // namespace
}  // namespace named
}  // namespace lhat
