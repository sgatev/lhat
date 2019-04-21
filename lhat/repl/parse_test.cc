#include "lhat/repl/parse.h"

#include <sstream>
#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace repl {
namespace {
TEST(ParseCommand, Empty) {
  const std::string expr = "";

  std::istringstream expr_stream(expr);
  const core::ParseResult<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());
  EXPECT_EQ(command.ConsumedChars(), expr.size());

  EXPECT_EQ(command.Value(), "");
}

TEST(ParseCommand, SkipsWhitespacePrefix) {
  const std::string expr = "   foo";

  std::istringstream expr_stream(expr);
  const core::ParseResult<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());
  EXPECT_EQ(command.ConsumedChars(), expr.size());

  EXPECT_EQ(command.Value(), "foo");
}

TEST(ParseCommand, ParsesUntilWhitespace) {
  const std::string expr = "   foo ";

  std::istringstream expr_stream(expr);
  const core::ParseResult<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());
  EXPECT_EQ(command.ConsumedChars(), 6);

  EXPECT_EQ(command.Value(), "foo");
}

TEST(ParseCommand, PermitsSpecialChars) {
  const std::string expr = "foo-bar?";

  std::istringstream expr_stream(expr);
  const core::ParseResult<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());
  EXPECT_EQ(command.ConsumedChars(), expr.size());

  EXPECT_EQ(command.Value(), "foo-bar?");
}
}  // namespace
}  // namespace repl
}  // namespace lhat
