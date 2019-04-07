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

TEST(InsertBoundVarNames, Var) {
  const Term term(Var("x"));

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, IsEmpty());
}

TEST(InsertBoundVarNames, Abst) {
  const Term term(Abst("x", Term(Var("y"))));

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertBoundVarNames, Appl) {
  const Term term(
      Appl(Term(Abst("x", Term(Var("y")))), Term(Abst("u", Term(Var("v"))))));

  std::unordered_set<std::string> var_names;
  InsertBoundVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "u"));
}

TEST(InsertFreeVarNames, Var) {
  const Term term(Var("x"));

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertFreeVarNames, Abst) {
  const Term term(Abst("x", Term(Var("y"))));

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("y"));
}

TEST(InsertFreeVarNames, AbstAlreadyExists) {
  const Term term(Abst("x", Term(Var("y"))));

  std::unordered_set<std::string> var_names = {"x"};
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y"));
}

TEST(InsertFreeVarNames, Appl) {
  const Term term(
      Appl(Term(Abst("x", Term(Var("y")))), Term(Abst("u", Term(Var("v"))))));

  std::unordered_set<std::string> var_names;
  InsertFreeVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("y", "v"));
}

TEST(InsertVarNames, Var) {
  const Term term(Var("x"));

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x"));
}

TEST(InsertVarNames, Abst) {
  const Term term(Abst("x", Term(Var("y"))));

  std::unordered_set<std::string> var_names;
  InsertVarNames(term, &var_names);

  EXPECT_THAT(var_names, UnorderedElementsAre("x", "y"));
}

TEST(InsertVarNames, Appl) {
  const Term term(
      Appl(Term(Abst("x", Term(Var("y")))), Term(Abst("u", Term(Var("v"))))));

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
