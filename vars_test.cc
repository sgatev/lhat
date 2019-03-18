#include "vars.h"

#include <string>
#include <unordered_set>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace {
using ::testing::IsEmpty;
using ::testing::UnorderedElementsAre;

TEST(InsertBoundVarNames, Null) {
  const std::shared_ptr<Term> term = nullptr;

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, IsEmpty());
}

TEST(InsertBoundVarNames, Var) {
  const auto term = std::make_shared<VarTerm>("x");

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, IsEmpty());
}

TEST(InsertBoundVarNames, Abst) {
  const auto term =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertBoundVarNames, Appl) {
  const auto term = std::make_shared<ApplTerm>(
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")),
      std::make_shared<AbstTerm>("u", std::make_shared<VarTerm>("v")));

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "u"));
}

TEST(InsertFreeVarNames, Null) {
  const std::shared_ptr<Term> term = nullptr;

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, IsEmpty());
}

TEST(InsertFreeVarNames, Var) {
  const auto term = std::make_shared<VarTerm>("x");

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertFreeVarNames, Abst) {
  const auto term =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("y"));
}

TEST(InsertFreeVarNames, AbstAlreadyExists) {
  const auto term =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));

  std::unordered_set<std::string> var_names = {"x"};
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y"));
}

TEST(InsertFreeVarNames, Appl) {
  const auto term = std::make_shared<ApplTerm>(
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")),
      std::make_shared<AbstTerm>("u", std::make_shared<VarTerm>("v")));

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("y", "v"));
}

TEST(InsertVarNames, Null) {
  const std::shared_ptr<Term> term = nullptr;

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, IsEmpty());
}

TEST(InsertVarNames, Var) {
  const auto term = std::make_shared<VarTerm>("x");

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertVarNames, Abst) {
  const auto term =
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y"));

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y"));
}

TEST(InsertVarNames, Appl) {
  const auto term = std::make_shared<ApplTerm>(
      std::make_shared<AbstTerm>("x", std::make_shared<VarTerm>("y")),
      std::make_shared<AbstTerm>("u", std::make_shared<VarTerm>("v")));

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y", "u", "v"));
}
}  // namespace
}  // namespace lhat
