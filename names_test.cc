#include "names.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace {
TEST(NameContext, AddAndGetNames) {
  NameContext nctx;

  nctx.AddName("foo");
  nctx.AddName("bar");

  EXPECT_EQ(nctx.GetIndexForName("foo"), 0);
  EXPECT_EQ(nctx.GetNameForIndex(0), "foo");

  EXPECT_EQ(nctx.GetIndexForName("bar"), 1);
  EXPECT_EQ(nctx.GetNameForIndex(1), "bar");
}
}  // namespace
}  // namespace lhat
