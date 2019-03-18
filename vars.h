#ifndef LHAT_VARS_H
#define LHAT_VARS_H

#include <string>
#include <unordered_set>

#include "ast.h"

namespace lhat {
// InsertBoundVarsNames inserts the names of the bound variables of term in
// var_names.
void InsertBoundVarNames(const std::shared_ptr<Term> term,
                         std::unordered_set<std::string> *var_names);

// InsertFreeVarNames inserts the names of the free variables of term in
// var_names.
void InsertFreeVarNames(const std::shared_ptr<Term> term,
                        std::unordered_set<std::string> *var_names);

// InsertVarNames inserts the names of all variables of term in var_names.
void InsertVarNames(const std::shared_ptr<Term> term,
                    std::unordered_set<std::string> *var_names);

// NewVarName generates a variable name that is not present in var_names.
std::string NewVarName(std::unordered_set<std::string> var_names);
}  // namespace lhat

#endif  // LHAT_VARS_H
