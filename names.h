#ifndef LHAT_NAMES_H
#define LHAT_NAMES_H

#include <string>
#include <unordered_map>
#include <vector>

#include "named/ast.h"
#include "nameless/ast.h"

namespace lhat {
// NameContext is a context of names of lambda variables.
class NameContext {
public:
  // AddName adds name to the context and returns its index. If name is already
  // present in the context its index is returned.
  int AddName(const std::string& name);

  // GetNameForIndex returns the name that corresponds to index idx.
  std::string GetNameForIndex(int idx) const;

  // GetIndexForName returns the index that corresponds to name.
  int GetIndexForName(const std::string& name) const;

  // HasName returns true if the name is registered in the context.
  bool HasName(const std::string& name) const;

private:
  std::vector<std::string> names_;
  std::unordered_map<std::string, int> name_to_idx_;
};

// StripNames converts a named lambda term into a nameless lambda term.
std::shared_ptr<nameless::Term> StripNames(
    const std::shared_ptr<named::Term> term);
}  // namespace lhat

#endif  // LHAT_NAMES_H
