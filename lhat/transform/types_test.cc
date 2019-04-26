#include "lhat/transform/types.h"

#include <unordered_map>
#include <vector>

#include "lhat/nameless/ast.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace transform {
namespace {
using ::testing::ElementsAre;
using ::testing::IsEmpty;
using ::testing::Pointee;
using ::testing::SizeIs;
using ::testing::VariantWith;

TEST(CollectTypes, Var) {
  const nameless::Term& term = nameless::Var(1);
  const std::unordered_map<int, int> free_var_types = {{1, 21}};

  std::vector<Type> types;
  std::vector<TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      CollectTypes(term, free_var_types, &types, &constraints);

  EXPECT_THAT(types, SizeIs(1));
  EXPECT_THAT(types[0].Get<SimpleType>(), Pointee(SimpleType{21}));

  EXPECT_THAT(constraints, IsEmpty());

  EXPECT_EQ(term_type_idx.Value(), 0);
}

TEST(CollectTypes, Appl) {
  const nameless::Term& term =
      nameless::Appl(nameless::Var(1), nameless::Var(2));
  const std::unordered_map<int, int> free_var_types = {{1, 21}, {2, 42}};

  std::vector<Type> types;
  std::vector<TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      CollectTypes(term, free_var_types, &types, &constraints);

  EXPECT_THAT(types, SizeIs(6));
  EXPECT_THAT(types[0].Get<SimpleType>(), Pointee(SimpleType{21}));
  EXPECT_THAT(types[1].Get<SimpleType>(), Pointee(SimpleType{42}));
  EXPECT_THAT(types[2].Get<SimpleType>(), Pointee(SimpleType{2}));
  EXPECT_THAT(types[3].Get<SimpleType>(), Pointee(SimpleType{3}));
  EXPECT_THAT(types[4].Get<FuncType>(), Pointee(FuncType{2, 3}));
  EXPECT_THAT(types[5].Get<SimpleType>(), Pointee(SimpleType{5}));

  EXPECT_THAT(constraints,
              ElementsAre(TypeConstraint{0, 4}, TypeConstraint{1, 2},
                          TypeConstraint{5, 3}));

  EXPECT_EQ(term_type_idx.Value(), 5);
}

TEST(CollectTypes, AbstFreeVar) {
  const nameless::Term& term = nameless::Abst(nameless::Var(1));
  const std::unordered_map<int, int> free_var_types = {{0, 21}};

  std::vector<Type> types;
  std::vector<TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      CollectTypes(term, free_var_types, &types, &constraints);

  EXPECT_THAT(types, SizeIs(3));
  EXPECT_THAT(types[0].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[1].Get<SimpleType>(), Pointee(SimpleType{21}));
  EXPECT_THAT(types[2].Get<FuncType>(), Pointee(FuncType{0, 1}));

  EXPECT_THAT(constraints, IsEmpty());

  EXPECT_EQ(term_type_idx.Value(), 2);
}

TEST(CollectTypes, AbstBoundVar) {
  const nameless::Term& term = nameless::Abst(nameless::Var(-1));
  const std::unordered_map<int, int> free_var_types = {};

  std::vector<Type> types;
  std::vector<TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      CollectTypes(term, free_var_types, &types, &constraints);

  EXPECT_THAT(types, SizeIs(3));
  EXPECT_THAT(types[0].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[1].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[2].Get<FuncType>(), Pointee(FuncType{0, 1}));

  EXPECT_THAT(constraints, IsEmpty());

  EXPECT_EQ(term_type_idx.Value(), 2);
}

TEST(CollectTypes, NestedAbst) {
  const nameless::Term& term =
      nameless::Abst(nameless::Term(nameless::Abst(nameless::Var(-2))));
  const std::unordered_map<int, int> free_var_types = {};

  std::vector<Type> types;
  std::vector<TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      CollectTypes(term, free_var_types, &types, &constraints);

  EXPECT_THAT(types, SizeIs(5));
  EXPECT_THAT(types[0].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[1].Get<SimpleType>(), Pointee(SimpleType{1}));
  EXPECT_THAT(types[2].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[3].Get<FuncType>(), Pointee(FuncType{1, 2}));
  EXPECT_THAT(types[4].Get<FuncType>(), Pointee(FuncType{0, 3}));

  EXPECT_THAT(constraints, IsEmpty());

  EXPECT_EQ(term_type_idx.Value(), 4);
}

TEST(CollectTypes, NestedAbstAppl) {
  const nameless::Term& term = nameless::Abst(nameless::Term(
      nameless::Abst(nameless::Appl(nameless::Var(-1), nameless::Var(-2)))));
  const std::unordered_map<int, int> free_var_types = {};

  std::vector<Type> types;
  std::vector<TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      CollectTypes(term, free_var_types, &types, &constraints);

  EXPECT_THAT(types, SizeIs(10));
  EXPECT_THAT(types[0].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[1].Get<SimpleType>(), Pointee(SimpleType{1}));
  EXPECT_THAT(types[2].Get<SimpleType>(), Pointee(SimpleType{1}));
  EXPECT_THAT(types[3].Get<SimpleType>(), Pointee(SimpleType{0}));
  EXPECT_THAT(types[4].Get<SimpleType>(), Pointee(SimpleType{4}));
  EXPECT_THAT(types[5].Get<SimpleType>(), Pointee(SimpleType{5}));
  EXPECT_THAT(types[6].Get<FuncType>(), Pointee(FuncType{4, 5}));
  EXPECT_THAT(types[7].Get<SimpleType>(), Pointee(SimpleType{7}));
  EXPECT_THAT(types[8].Get<FuncType>(), Pointee(FuncType{1, 7}));
  EXPECT_THAT(types[9].Get<FuncType>(), Pointee(FuncType{0, 8}));

  EXPECT_THAT(constraints,
              ElementsAre(TypeConstraint{2, 6}, TypeConstraint{3, 4},
                          TypeConstraint{7, 5}));

  EXPECT_EQ(term_type_idx.Value(), 9);
}
}  // namespace
}  // namespace transform
}  // namespace lhat
