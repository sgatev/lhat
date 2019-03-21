#include "vars.h"

#include <string>
#include <unordered_set>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace named {
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
  const auto term = VarTerm::Make("x");

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, IsEmpty());
}

TEST(InsertBoundVarNames, Abst) {
  const auto term = AbstTerm::Make("x", VarTerm::Make("y"));

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertBoundVarNames, Appl) {
  const auto term = ApplTerm::Make(AbstTerm::Make("x", VarTerm::Make("y")),
                                   AbstTerm::Make("u", VarTerm::Make("v")));

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
  const auto term = VarTerm::Make("x");

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertFreeVarNames, Abst) {
  const auto term = AbstTerm::Make("x", VarTerm::Make("y"));

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("y"));
}

TEST(InsertFreeVarNames, AbstAlreadyExists) {
  const auto term = AbstTerm::Make("x", VarTerm::Make("y"));

  std::unordered_set<std::string> var_names = {"x"};
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y"));
}

TEST(InsertFreeVarNames, Appl) {
  const auto term = ApplTerm::Make(AbstTerm::Make("x", VarTerm::Make("y")),
                                   AbstTerm::Make("u", VarTerm::Make("v")));

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
  const auto term = VarTerm::Make("x");

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertVarNames, Abst) {
  const auto term = AbstTerm::Make("x", VarTerm::Make("y"));

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y"));
}

TEST(InsertVarNames, Appl) {
  const auto term = ApplTerm::Make(AbstTerm::Make("x", VarTerm::Make("y")),
                                   AbstTerm::Make("u", VarTerm::Make("v")));

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y", "u", "v"));
}

TEST(NewVarName, Empty) {
  const std::unordered_set<std::string> var_names;
  EXPECT_EQ(NewVarName(var_names), "a");
}

TEST(NewVarName, Taken) {
  const std::unordered_set<std::string> var_names = {
      "a",
      "b",
      "d",
      "e",
  };
  EXPECT_EQ(NewVarName(var_names), "c");
}

TEST(NewVarName, AllSingleLetterTaken) {
  std::unordered_set<std::string> var_names = {};
  for (char c = 'a'; c <= 'z'; c++) {
    var_names.insert(std::string(1, c));
  }
  EXPECT_EQ(NewVarName(var_names), "aa");
}
}  // namespace
}  // namespace named
}  // namespace lhat
