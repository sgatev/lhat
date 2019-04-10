#include "const_env.h"

#include <vector>

#include "absl/strings/str_replace.h"
#include "absl/strings/string_view.h"

namespace lhat {
namespace repl {
void ConstEnv::Set(std::string name, std::string value) {
  Resolve(&value);
  vars_.emplace(std::make_pair(name, value));
}

void ConstEnv::Resolve(std::string* s) {
  std::vector<std::pair<std::string, std::string>> replacements;

  std::smatch matches;
  std::string::const_iterator s_it(s->cbegin());
  while (std::regex_search(s_it, s->cend(), matches, name_regex_)) {
    std::string name(matches.str(0));
    replacements.push_back({name, vars_[name.substr(1)]});
    s_it = matches.suffix().first;
  }

  absl::StrReplaceAll(replacements, s);
}
}  // namespace repl
}  // namespace lhat
