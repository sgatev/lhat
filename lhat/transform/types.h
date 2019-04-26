#ifndef LHAT_TRANSFORM_TYPES_H
#define LHAT_TRANSFORM_TYPES_H

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "lhat/nameless/ast.h"
#include "lhat/util/error.h"

namespace lhat {
namespace transform {
// A simple lambda type.
class SimpleType {
 public:
  SimpleType(int id);

  SimpleType(const SimpleType& other) = default;
  SimpleType& operator=(const SimpleType& other) = default;
  SimpleType(SimpleType&& other) = default;
  SimpleType& operator=(SimpleType&& other) = default;

  // Tests for equality with another simple type.
  bool operator==(const SimpleType& other) const;

  // Returns the id of the type.
  int Id() const;

  // Returns the name of the type.
  std::string Name() const;

 private:
  int id_;
};

// A lambda function type.
class FuncType {
 public:
  FuncType(int var_idx, int res_idx);

  FuncType(const FuncType& other) = default;
  FuncType& operator=(const FuncType& other) = default;
  FuncType(FuncType&& other) = default;
  FuncType& operator=(FuncType&& other) = default;

  // Tests for equality with another lambda function type.
  bool operator==(const FuncType& other) const;

  // Returns the index of the function variable type.
  int VarIndex() const;

  // Returns the index of the function result type.
  int ResIndex() const;

 private:
  int var_idx_;
  int res_idx_;
};

// Constraint of a variable lambda type.
class TypeConstraint {
 public:
  TypeConstraint(int var_idx, int val_idx);

  TypeConstraint(const TypeConstraint& other) = default;
  TypeConstraint& operator=(const TypeConstraint& other) = default;
  TypeConstraint(TypeConstraint&& other) = default;
  TypeConstraint& operator=(TypeConstraint&& other) = default;

  // Tests for equality with another type alias.
  bool operator==(const TypeConstraint& other) const;

  // Returns the index of the variable type.
  int VarIndex() const;

  // Returns the index of the value type.
  int ValIndex() const;

 private:
  int var_idx_;
  int val_idx_;
};

// Kinds of lambda types.
enum TypeKind {
  SIMPLE,
  FUNC,
};

// A lambda type.
class Type {
 public:
  Type(SimpleType type);
  Type(FuncType type);

  Type(const Type& term) = default;
  Type(Type&& term) = default;
  Type& operator=(const Type& other) = default;
  Type& operator=(Type&& other) = default;

  // Returns the kind of the lambda type.
  TypeKind Kind() const;

  // Returns the concrete instance of the lambda type.
  template <class T>
  constexpr auto Get() & -> decltype(auto) {
    return std::get_if<T>(&type_);
  }

  // Returns the concrete instance of the lambda type.
  template <class T>
  constexpr auto Get() const& -> decltype(auto) {
    return std::get_if<T>(&type_);
  }

  // Passes the type to a set of type-based matchers and executes the one that
  // matches.
  template <class... M>
  constexpr auto Match(M&&... matchers) & -> decltype(auto) {
    return std::visit(util::Overload(std::forward<M>(matchers)...), type_);
  }

  // Passes the type to a set of type-based matchers and executes the one that
  // matches.
  template <class... M>
  constexpr auto Match(M&&... matchers) const& -> decltype(auto) {
    return std::visit(util::Overload(std::forward<M>(matchers)...), type_);
  }

 private:
  std::variant<SimpleType, FuncType> type_;
};

// Collects the types and constraints of all subterms of term and returns the
// index of the type of term in types. Types of all free variables in term
// should be provided in free_var_types.
util::ErrorOr<int> CollectTypes(
    const nameless::Term& term,
    const std::unordered_map<int, int>& free_var_types,
    std::vector<Type>* types, std::vector<TypeConstraint>* constraints);

// Performs type unification and returns all required type substitutions in
// subs.
bool UnifyTypes(const std::vector<Type>& types,
                const std::vector<TypeConstraint>& constraints,
                std::unordered_map<SimpleType, int>* subs);

// Applies the provided type substitutions to types.
void ApplyTypeSubs(const std::unordered_map<SimpleType, int>& subs,
                   std::vector<Type>* types);

// Converts the type at index idx to string.
std::string TypeToString(const std::vector<Type>& types, int idx);
}  // namespace transform
}  // namespace lhat

namespace std {
template <>
struct hash<lhat::transform::SimpleType> {
  size_t operator()(const lhat::transform::SimpleType& id) const {
    return hash<int>()(id.Id());
  }
};
}  // namespace std

#endif  // LHAT_TRANSFORM_TYPES_H
