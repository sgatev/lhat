#ifndef LHAT_NAMED_VARS_H
#define LHAT_NAMED_VARS_H

#include <string>
#include <unordered_set>

#include "ast.h"

namespace lhat {
namespace named {
// InsertBoundVarsNames inserts the names of the bound variables of term in
// var_names.
void InsertBoundVarNames(const Term& term,
                         std::unordered_set<std::string>* var_names);

// InsertFreeVarNames inserts the names of the free variables of term in
// var_names.
void InsertFreeVarNames(const Term& term,
                        std::unordered_set<std::string>* var_names);

// InsertVarNames inserts the names of all variables of term in var_names.
void InsertVarNames(const Term& term,
                    std::unordered_set<std::string>* var_names);

// NewVarName generates a variable name that is not present in var_names.
std::string NewVarName(const std::unordered_set<std::string>& var_names);
}  // namespace named
}  // namespace lhat

#endif  // LHAT_NAMED_VARS_H
