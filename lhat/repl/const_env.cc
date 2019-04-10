#include "lhat/repl/const_env.h"

#include <vector>

#include "absl/strings/str_replace.h"
#include "absl/strings/string_view.h"

namespace lhat {
namespace repl {
void ConstEnv::Set(std::pair<std::string, std::string> name_value) {
  Resolve(&name_value.second);
  vars_.emplace(name_value);
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
