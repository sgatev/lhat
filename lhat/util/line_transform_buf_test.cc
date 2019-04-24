#include "lhat/util/line_transform_buf.h"

#include <istream>
#include <sstream>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace util {
namespace {
using ::testing::IsEmpty;

TEST(LineTransformBuf, TransformsLines) {
  std::istringstream stream("foo\nbar");
  LineTransformBuf transform_buf(&stream, [](std::string* line) -> bool {
    if (*line == "foo") {
      *line = "boo";
    } else {
      *line = "far";
    }
    return true;
  });
  std::istream transformed_stream(&transform_buf);

  std::string foo_line;
  std::getline(transformed_stream, foo_line);
  EXPECT_EQ(foo_line, "boo");

  std::string bar_line;
  std::getline(transformed_stream, bar_line);
  EXPECT_EQ(bar_line, "far");

  EXPECT_TRUE(transformed_stream.eof());
}

TEST(LineTransformBuf, TerminatesStream) {
  std::istringstream stream("foo\nbar\nqux");
  LineTransformBuf transform_buf(&stream, [](std::string* line) -> bool {
    if (*line == "bar") {
      return false;
    }
    return true;
  });
  std::istream transformed_stream(&transform_buf);

  std::string foo_line;
  std::getline(transformed_stream, foo_line);
  EXPECT_EQ(foo_line, "foo");

  std::string bar_line;
  std::getline(transformed_stream, bar_line);
  EXPECT_THAT(bar_line, IsEmpty());

  EXPECT_TRUE(transformed_stream.eof());
}
}  // namespace
}  // namespace util
}  // namespace lhat
