#ifndef LHAT_UTIL_PARSE_H
#define LHAT_UTIL_PARSE_H

#include <istream>

namespace lhat::util {

// Discards all starting whitespaces from input.
void DiscardWhitespace(std::istream* input);

}  // namespace lhat::util

#endif  // LHAT_UTIL_PARSE_H
