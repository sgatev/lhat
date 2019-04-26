#include "lhat/transform/types.h"

#include "absl/strings/str_cat.h"

namespace lhat {
namespace transform {
namespace {
util::ErrorOr<int> CollectTypes(
    const nameless::Term& term,
    const std::unordered_map<int, int>& free_var_types, int abst_count,
    std::vector<Type>* types, std::vector<TypeConstraint>* constraints,
    std::vector<int>* bound_var_types) {
  return term.Match(
      [abst_count, &free_var_types, types, constraints,
       bound_var_types](const nameless::Abst& abst) -> util::ErrorOr<int> {
        // Add function variable type.
        const int var_type_idx = types->size();
        types->push_back(SimpleType{var_type_idx});

        bound_var_types->push_back(var_type_idx);

        // Get function result type.
        const util::ErrorOr<int> res_type_idx =
            CollectTypes(abst.Body(), free_var_types, abst_count + 1, types,
                         constraints, bound_var_types);
        if (!res_type_idx.Ok()) {
          return res_type_idx.Error();
        }

        bound_var_types->pop_back();

        // Add function type.
        const int type_idx = types->size();
        types->push_back(FuncType{var_type_idx, res_type_idx.Value()});

        return type_idx;
      },
      [abst_count, &free_var_types, types, constraints,
       bound_var_types](const nameless::Appl& appl) -> util::ErrorOr<int> {
        const util::ErrorOr<int> func_type_idx =
            CollectTypes(appl.Func(), free_var_types, abst_count, types,
                         constraints, bound_var_types);
        if (!func_type_idx.Ok()) {
          return func_type_idx.Error();
        }

        const util::ErrorOr<int> arg_type_idx =
            CollectTypes(appl.Arg(), free_var_types, abst_count, types,
                         constraints, bound_var_types);
        if (!arg_type_idx.Ok()) {
          return arg_type_idx.Error();
        }

        const int func_var_type_idx = types->size();
        types->push_back(SimpleType{func_var_type_idx});

        const int func_result_type_idx = types->size();
        types->push_back(SimpleType{func_result_type_idx});

        const int new_func_type_idx = types->size();
        types->push_back(FuncType{func_var_type_idx, func_result_type_idx});
        constraints->push_back(
            TypeConstraint{func_type_idx.Value(), new_func_type_idx});

        constraints->push_back(
            TypeConstraint{arg_type_idx.Value(), func_var_type_idx});

        // Add application type and constraint based on function result.
        const int type_idx = types->size();
        types->push_back(SimpleType{type_idx});
        constraints->push_back(TypeConstraint{type_idx, func_result_type_idx});

        return type_idx;
      },
      [abst_count, &free_var_types, types,
       bound_var_types](const nameless::Var& var) -> util::ErrorOr<int> {
        const int var_type_idx = types->size();
        if (var.Index() < 0) {
          types->push_back(
              SimpleType{bound_var_types->at(abst_count + var.Index())});
        } else {
          if (free_var_types.find(var.Index() - abst_count) ==
              free_var_types.end()) {
            return util::Error(absl::StrCat("Missing free var type: ",
                                            var.Index() - abst_count));
          }
          types->push_back(
              SimpleType{free_var_types.at(var.Index() - abst_count)});
        }
        return var_type_idx;
      });
}

bool UnifyTypes(const std::vector<Type>& types, int left, int right,
                std::unordered_map<SimpleType, int>* subs);

bool OccursCheck(const std::vector<Type>& types, int var_idx, int right_idx,
                 std::unordered_map<SimpleType, int>* subs) {
  return types[right_idx].Match(
      [&types, var_idx, subs](const SimpleType& right) -> bool {
        if (subs->find(right) == subs->end()) {
          return false;
        }
        return OccursCheck(types, var_idx, subs->at(right), subs);
      },
      [&types, var_idx, subs](const FuncType& right) -> bool {
        return OccursCheck(types, var_idx, right.VarIndex(), subs) ||
               OccursCheck(types, var_idx, right.ResIndex(), subs);
      });
}

bool UnifySimpleType(const std::vector<Type>& types, int var_idx, int right_idx,
                     std::unordered_map<SimpleType, int>* subs) {
  const SimpleType* var = types[var_idx].Get<SimpleType>();
  if (subs->find(*var) != subs->end()) {
    return UnifyTypes(types, subs->at(*var), right_idx, subs);
  }
  if (types[right_idx].Kind() == SIMPLE) {
    const SimpleType* right = types[right_idx].Get<SimpleType>();
    if (subs->find(*right) != subs->end()) {
      return UnifyTypes(types, var_idx, subs->at(*right), subs);
    }
  }
  if (OccursCheck(types, var_idx, right_idx, subs)) {
    return false;
  }
  subs->insert({*var, right_idx});
  return true;
}

bool UnifyTypes(const std::vector<Type>& types, int left_idx, int right_idx,
                std::unordered_map<SimpleType, int>* subs) {
  if (types[left_idx].Kind() == SIMPLE) {
    return UnifySimpleType(types, left_idx, right_idx, subs);
  } else if (types[right_idx].Kind() == SIMPLE) {
    return UnifySimpleType(types, right_idx, left_idx, subs);
  } else if (types[left_idx].Kind() == FUNC &&
             types[right_idx].Kind() == FUNC) {
    const FuncType* left_func = types[left_idx].Get<FuncType>();
    const FuncType* right_func = types[right_idx].Get<FuncType>();
    return UnifyTypes(types, left_func->VarIndex(), right_func->VarIndex(),
                      subs) &&
           UnifyTypes(types, left_func->ResIndex(), right_func->ResIndex(),
                      subs);
  }
  return false;
}
}  // namespace

SimpleType::SimpleType(int id) : id_(id) {}

bool SimpleType::operator==(const SimpleType& other) const {
  return id_ == other.id_;
}

int SimpleType::Id() const { return id_; }

std::string SimpleType::Name() const { return absl::StrCat("T", id_); }

FuncType::FuncType(int var_idx, int res_idx)
    : var_idx_(var_idx), res_idx_(res_idx) {}

bool FuncType::operator==(const FuncType& other) const {
  return var_idx_ == other.var_idx_ && res_idx_ == other.res_idx_;
}

int FuncType::VarIndex() const { return var_idx_; }

int FuncType::ResIndex() const { return res_idx_; }

Type::Type(SimpleType type) : type_(std::move(type)) {}

Type::Type(FuncType type) : type_(std::move(type)) {}

TypeKind Type::Kind() const { return static_cast<TypeKind>(type_.index()); }

TypeConstraint::TypeConstraint(int var_idx, int val_idx)
    : var_idx_(var_idx), val_idx_(val_idx) {}

bool TypeConstraint::operator==(const TypeConstraint& other) const {
  return var_idx_ == other.var_idx_ && val_idx_ == other.val_idx_;
}

int TypeConstraint::VarIndex() const { return var_idx_; }

int TypeConstraint::ValIndex() const { return val_idx_; }

util::ErrorOr<int> CollectTypes(
    const nameless::Term& term,
    const std::unordered_map<int, int>& free_var_types,
    std::vector<Type>* types, std::vector<TypeConstraint>* constraints) {
  std::vector<int> bound_var_types;
  return CollectTypes(term, free_var_types, 0, types, constraints,
                      &bound_var_types);
}

bool UnifyTypes(const std::vector<Type>& types,
                const std::vector<TypeConstraint>& constraints,
                std::unordered_map<SimpleType, int>* subs) {
  for (const TypeConstraint constraint : constraints) {
    if (!UnifyTypes(types, constraint.VarIndex(), constraint.ValIndex(),
                    subs)) {
      return false;
    }
  }
  return true;
}

void ApplyTypeSubs(const std::unordered_map<SimpleType, int>& subs,
                   std::vector<Type>* types) {
  for (Type& type : *types) {
    const SimpleType* simple_type = type.Get<SimpleType>();
    if (simple_type != nullptr && (subs.find(*simple_type) != subs.end())) {
      type = types->at(subs.at(*simple_type));
    }
  }
}

std::string TypeToString(const std::vector<Type>& types, int idx) {
  return types[idx].Match(
      [](const SimpleType& type) -> std::string { return type.Name(); },
      [&types](const FuncType& type) -> std::string {
        if (types[type.VarIndex()].Kind() == FUNC) {
          return absl::StrCat("(", TypeToString(types, type.VarIndex()), ")",
                              " -> ", TypeToString(types, type.ResIndex()));
        } else {
          return absl::StrCat(TypeToString(types, type.VarIndex()), " -> ",
                              TypeToString(types, type.ResIndex()));
        }
      });
}
}  // namespace transform
}  // namespace lhat
