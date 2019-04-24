#include "lhat/repl/const_env.h"

#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace repl {
namespace {
TEST(ConstEnv, NoConst) {
  ConstEnv consts;
  consts.Set("K", "hello");

  std::string s = "world";
  EXPECT_FALSE(consts.Resolve(&s));

  EXPECT_EQ(s, "world");
}

TEST(ConstEnv, SingleConst) {
  ConstEnv consts;
  consts.Set("K", "hello");

  std::string s = "'K world";
  EXPECT_FALSE(consts.Resolve(&s));

  EXPECT_EQ(s, "hello world");
}

TEST(ConstEnv, MultipleConsts) {
  ConstEnv consts;
  consts.Set("M", "hello");
  consts.Set("N", "world");

  std::string s = "'M 'N";
  EXPECT_FALSE(consts.Resolve(&s));

  EXPECT_EQ(s, "hello world");
}

TEST(ConstEnv, NestedConsts) {
  ConstEnv consts;
  consts.Set("M", "hello");
  consts.Set("N", "'M world");

  std::string s = "'N";
  EXPECT_FALSE(consts.Resolve(&s));

  EXPECT_EQ(s, "hello world");
}

TEST(ConstEnv, PrefixConst) {
  ConstEnv consts;
  consts.Set("K", "hello");
  consts.Set("KK", "world");

  std::string s = "'KK";
  EXPECT_FALSE(consts.Resolve(&s));

  EXPECT_EQ(s, "world");
}

TEST(ConstEnv, NotRegistered) {
  ConstEnv consts;

  std::string s = "'M";
  const std::optional<util::Error> resolve_error = consts.Resolve(&s);
  EXPECT_TRUE(resolve_error);
  EXPECT_EQ("M is not registered", resolve_error->Message());

  EXPECT_EQ(s, "'M");
}
}  // namespace
}  // namespace repl
}  // namespace lhat
