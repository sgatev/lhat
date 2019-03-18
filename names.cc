#include "names.h"

namespace lhat {
int NameContext::AddName(const std::string& name) {
  const auto name_idx = name_to_idx_.find(name);
  if (name_idx != name_to_idx_.end()) {
    // Name is already added.
    return name_idx->second;
  }

  int idx = names_.size();
  name_to_idx_[name] = idx;
  names_.push_back(name);
  return idx;
}

std::string NameContext::GetNameForIndex(int idx) const { return names_[idx]; }

int NameContext::GetIndexForName(const std::string& name) const {
  return name_to_idx_.at(name);
}
}  // namespace lhat
