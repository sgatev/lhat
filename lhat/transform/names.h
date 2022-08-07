#ifndef LHAT_TRANSFORM_NAMES_H
#define LHAT_TRANSFORM_NAMES_H

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "lhat/named/ast.h"
#include "lhat/nameless/ast.h"

namespace lhat::transform {

// A context of names of lambda variables.
class NameContext {
 public:
  NameContext();

  // Adds name to the context and returns its index. If name is already present
  // in the context its index is returned.
  int AddName(const std::string& name);

  // Sets the name at the specified index in the context and returns its
  // index.
  void SetName(const std::string& name, int idx);

  // Returns the name that corresponds to index idx.
  std::string GetNameForIndex(int idx) const;

  // Returns the index that corresponds to name.
  int GetIndexForName(const std::string& name) const;

  // Returns true if the name is registered in the context.
  bool HasName(const std::string& name) const;

  // Returns true if a name is already register at the specified index in the
  // context.
  bool HasIndex(int idx) const;

  // Returns the names registered in the context.
  std::unordered_set<std::string> Names() const;

 private:
  std::unordered_map<int, std::string> idx_to_name_;
  std::unordered_map<std::string, int> name_to_idx_;
  int idx_;
};

// Converts a nameless lambda term into a named lambda term. Uses the names
// registered in free_nctx for free vars.
named::Term AddNames(const nameless::Term& term, NameContext* free_nctx);

// Converts a named lambda term into a nameless lambda term. Additionally,
// registers the names of term's free vars in free_nctx.
nameless::Term RemoveNames(const named::Term& term, NameContext* free_nctx);

}  // namespace lhat::transform

#endif  // LHAT_TRANSFORM_NAMES_H
