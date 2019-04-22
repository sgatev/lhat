#include "lhat/util/line_transform_buf.h"

#include <istream>
#include <sstream>
#include <string>

#include "gtest/gtest.h"

namespace lhat {
namespace util {
namespace {
TEST(LineTransformBuf, TransformsLines) {
  std::istringstream stream("foo\nbar");
  LineTransformBuf transform_buf(&stream, [](std::string* line) {
    if (*line == "foo") {
      *line = "boo";
    } else {
      *line = "far";
    }
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

}  // namespace
}  // namespace util
}  // namespace lhat
