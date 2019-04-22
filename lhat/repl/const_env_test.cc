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
  consts.Resolve(&s);

  EXPECT_EQ(s, "world");
}

TEST(ConstEnv, SingleConst) {
  ConstEnv consts;
  consts.Set("K", "hello");

  std::string s = "'K world";
  consts.Resolve(&s);

  EXPECT_EQ(s, "hello world");
}

TEST(ConstEnv, MultipleConsts) {
  ConstEnv consts;
  consts.Set("M", "hello");
  consts.Set("N", "world");

  std::string s = "'M 'N";
  consts.Resolve(&s);

  EXPECT_EQ(s, "hello world");
}

TEST(ConstEnv, NestedConsts) {
  ConstEnv consts;
  consts.Set("M", "hello");
  consts.Set("N", "'M world");

  std::string s = "'N";
  consts.Resolve(&s);

  EXPECT_EQ(s, "hello world");
}

TEST(ConstEnv, PrefixConst) {
  ConstEnv consts;
  consts.Set("K", "hello");
  consts.Set("KK", "world");

  std::string s = "'KK";
  consts.Resolve(&s);

  EXPECT_EQ(s, "world");
}
}  // namespace
}  // namespace repl
}  // namespace lhat
