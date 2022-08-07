#include "lhat/util/parse.h"

#include "gtest/gtest.h"

namespace lhat::util {
namespace {

TEST(DiscardWhitespace, DiscardsStartingWhitespace) {
  std::istringstream stream("  \t \n foo  \t \n");
  DiscardWhitespace(&stream);
  const std::string result(std::istreambuf_iterator<char>(stream), {});
  EXPECT_EQ(result, "foo  \t \n");
}

}  // namespace
}  // namespace lhat::util
