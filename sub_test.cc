#include "sub.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace {
using ::testing::IsNull;
using ::testing::NotNull;

TEST(Sub, Null) {
  const std::shared_ptr<Term> result =
      Sub(nullptr, "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, IsNull());
}

TEST(Sub, SameVar) {
  const std::shared_ptr<Term> result =
      Sub(std::make_shared<VarTerm>("x"), "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(result);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->name, "y");
}

TEST(Sub, OtherVar) {
  const std::shared_ptr<Term> result =
      Sub(std::make_shared<VarTerm>("z"), "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(result);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->name, "z");
}

TEST(Sub, AbstSameVar) {
  const std::shared_ptr<Term> result =
      Sub(std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("x")), "x",
          std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(result);
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->var_name, "x");

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->name, "x");
}

TEST(Sub, AbstOtherVar) {
  const std::shared_ptr<Term> result =
      Sub(std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")), "y",
          std::make_shared<VarTerm>("z"));
  ASSERT_THAT(result, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(result);
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->var_name, "x");

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->name, "z");
}

TEST(Sub, Appl) {
  const std::shared_ptr<Term> result =
      Sub(std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                     std::make_shared<VarTerm>("x")),
          "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(result);
  ASSERT_THAT(appl, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->name, "y");

  const auto arg_var = std::static_pointer_cast<VarTerm>(appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->name, "y");
}

TEST(SafeSub, Null) {
  const std::shared_ptr<Term> result =
      SafeSub(nullptr, "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, IsNull());
}

TEST(SafeSub, SameVar) {
  const std::shared_ptr<Term> result = SafeSub(
      std::make_shared<VarTerm>("x"), "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(result);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->name, "y");
}

TEST(SafeSub, OtherVar) {
  const std::shared_ptr<Term> result = SafeSub(
      std::make_shared<VarTerm>("z"), "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto var = std::static_pointer_cast<VarTerm>(result);
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->name, "z");
}

TEST(SafeSub, AbstSameVar) {
  const std::shared_ptr<Term> result =
      SafeSub(std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("x")),
              "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(result);
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->var_name, "x");

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->name, "x");
}

TEST(SafeSub, AbstOtherVar) {
  const std::shared_ptr<Term> result =
      SafeSub(std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")),
              "y", std::make_shared<VarTerm>("z"));
  ASSERT_THAT(result, NotNull());

  const auto abst = std::static_pointer_cast<AbstTerm>(result);
  ASSERT_THAT(abst, NotNull());

  EXPECT_EQ(abst->var_name, "x");

  const auto body_var = std::static_pointer_cast<VarTerm>(abst->body);
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->name, "z");
}

TEST(SafeSub, Appl) {
  const std::shared_ptr<Term> result =
      SafeSub(std::make_shared<ApplTerm>(std::make_shared<VarTerm>("x"),
                                         std::make_shared<VarTerm>("x")),
              "x", std::make_shared<VarTerm>("y"));
  ASSERT_THAT(result, NotNull());

  const auto appl = std::static_pointer_cast<ApplTerm>(result);
  ASSERT_THAT(appl, NotNull());

  const auto func_var = std::static_pointer_cast<VarTerm>(appl->func);
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->name, "y");

  const auto arg_var = std::static_pointer_cast<VarTerm>(appl->arg);
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->name, "y");
}
}  // namespace
}  // namespace lhat
