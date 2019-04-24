#include "lhat/repl/const_env.h"

#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"

namespace lhat {
namespace repl {
std::optional<util::Error> ConstEnv::Set(std::string name, std::string value) {
  const std::optional<util::Error> resolve_error = Resolve(&value);
  if (resolve_error) {
    return resolve_error;
  }
  vars_.emplace(std::make_pair(name, value));
  return std::nullopt;
}

std::optional<util::Error> ConstEnv::Resolve(std::string* s) const {
  std::vector<std::pair<std::string, std::string>> replacements;

  std::smatch matches;
  std::string::const_iterator s_it(s->cbegin());
  while (std::regex_search(s_it, s->cend(), matches, name_regex_)) {
    const std::string const_ref(matches.str(0));
    const std::string const_name = const_ref.substr(1);

    if (vars_.find(const_name) == vars_.end()) {
      return util::Error(absl::StrCat(const_name, " is not registered"));
    }

    replacements.push_back({const_ref, vars_.at(const_name)});
    s_it = matches.suffix().first;
  }

  absl::StrReplaceAll(replacements, s);

  return std::nullopt;
}
}  // namespace repl
}  // namespace lhat
