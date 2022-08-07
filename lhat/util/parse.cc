#include "lhat/util/parse.h"

#include <cctype>

namespace lhat::util {

void DiscardWhitespace(std::istream* input) {
  while (!input->eof() && std::isspace(input->peek())) {
    input->get();
  }
}

}  // namespace lhat::util
