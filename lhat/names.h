#ifndef LHAT_NAMES_H
#define LHAT_NAMES_H

#include <string>
#include <unordered_map>

#include "named/ast.h"
#include "nameless/ast.h"

namespace lhat {
// NameContext is a context of names of lambda variables.
class NameContext {
public:
  NameContext();

  // AddName adds name to the context and returns its index. If name is already
  // present in the context its index is returned.
  int AddName(const std::string& name);

  // SetName sets the name at the specified index in the context and returns its
  // index.
  void SetName(const std::string& name, int idx);

  // GetNameForIndex returns the name that corresponds to index idx.
  std::string GetNameForIndex(int idx) const;

  // GetIndexForName returns the index that corresponds to name.
  int GetIndexForName(const std::string& name) const;

  // HasName returns true if the name is registered in the context.
  bool HasName(const std::string& name) const;

  // HasIndex returns true if a name is already register at the specified index
  // in the context.
  bool HasIndex(int idx) const;

private:
  std::unordered_map<int, std::string> idx_to_name_;
  std::unordered_map<std::string, int> name_to_idx_;
  int idx_;
};

// AddNames converts a nameless lambda term into a named lambda term.
named::Term AddNames(const nameless::Term& term);

// RemoveNames converts a named lambda term into a nameless lambda term.
nameless::Term RemoveNames(const named::Term& term);
}  // namespace lhat

#endif  // LHAT_NAMES_H
