#include "lhat/repl/parse.h"

#include <sstream>
#include <string>

#include "gtest/gtest.h"

namespace lhat::repl {
namespace {

TEST(ParseCommand, Empty) {
  const std::string expr = "";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());

  EXPECT_EQ(command.Value(), "");
}

TEST(ParseCommand, SkipsWhitespacePrefix) {
  const std::string expr = "   foo";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());

  EXPECT_EQ(command.Value(), "foo");
}

TEST(ParseCommand, ParsesUntilWhitespace) {
  const std::string expr = "   foo ";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());

  EXPECT_EQ(command.Value(), "foo");
}

TEST(ParseCommand, PermitsSpecialChars) {
  const std::string expr = "foo-bar?";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> command = ParseCommand(&expr_stream);
  EXPECT_TRUE(command.Ok());

  EXPECT_EQ(command.Value(), "foo-bar?");
}

TEST(ParseConstName, Empty) {
  const std::string expr = "";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> const_name = ParseConstName(&expr_stream);
  EXPECT_TRUE(const_name.Ok());

  EXPECT_EQ(const_name.Value(), "");
}

TEST(ParseConstName, SkipsWhitespacePrefix) {
  const std::string expr = "   foo";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> const_name = ParseConstName(&expr_stream);
  EXPECT_TRUE(const_name.Ok());

  EXPECT_EQ(const_name.Value(), "foo");
}

TEST(ParseConstName, ParsesUntilWhitespace) {
  const std::string expr = "   foo ";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> const_name = ParseConstName(&expr_stream);
  EXPECT_TRUE(const_name.Ok());

  EXPECT_EQ(const_name.Value(), "foo");
}

TEST(ParseConstName, PermitsSpecialChars) {
  const std::string expr = "foo*";

  std::istringstream expr_stream(expr);
  const util::ErrorOr<std::string> const_name = ParseConstName(&expr_stream);
  EXPECT_TRUE(const_name.Ok());

  EXPECT_EQ(const_name.Value(), "foo*");
}

}  // namespace
}  // namespace lhat::repl
