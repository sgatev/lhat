#include "lhat/util/parse.h"

namespace lhat {
namespace util {
namespace {
bool IsWhitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
}  // namespace

void DiscardWhitespace(std::istream* input) {
  while (!input->eof() && IsWhitespace(input->peek())) {
    input->get();
  }
}
}  // namespace util
}  // namespace lhat
