#ifndef LHAT_REPL_CONST_ENV_H
#define LHAT_REPL_CONST_ENV_H

#include <regex>
#include <string>
#include <unordered_map>

namespace lhat {
namespace repl {
// Environment of constant definitions.
class ConstEnv {
 public:
  // Sets the constant with the specified name to value.
  void Set(std::string name, std::string value);

  // Replaces the constants in s with their values.
  void Resolve(std::string *s) const;

 private:
  const std::regex name_regex_{R"('[a-zA-Z0-9*+-_!?]+)", std::regex::extended};

  std::unordered_map<std::string, std::string> vars_;
};
}  // namespace repl
}  // namespace lhat

#endif  // LHAT_REPL_CONST_ENV_H
