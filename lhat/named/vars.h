#ifndef LHAT_NAMED_VARS_H
#define LHAT_NAMED_VARS_H

#include <string>
#include <unordered_set>

#include "lhat/named/ast.h"

namespace lhat::named {

// Inserts the names of the bound variables of term in var_names.
void InsertBoundVarNames(const Term& term,
                         std::unordered_set<std::string>* var_names);

// Inserts the names of the free variables of term in var_names.
void InsertFreeVarNames(const Term& term,
                        std::unordered_set<std::string>* var_names);

// Inserts the names of all variables of term in var_names.
void InsertVarNames(const Term& term,
                    std::unordered_set<std::string>* var_names);

// Generates a variable name that is not present in var_names.
std::string NewVarName(const std::unordered_set<std::string>& var_names);

}  // namespace lhat::named

#endif  // LHAT_NAMED_VARS_H
