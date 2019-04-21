#include "lhat/core/parse.h"

namespace lhat {
namespace core {
namespace {
bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
}  // namespace

void ConsumeWhitespace(std::istream* input) {
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
  }
}
}  // namespace core
}  // namespace lhat
