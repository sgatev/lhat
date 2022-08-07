#ifndef LHAT_UTIL_LINE_TRANSFORM_BUF_H
#define LHAT_UTIL_LINE_TRANSFORM_BUF_H

#include <istream>
#include <streambuf>
#include <string>

namespace lhat::util {

// A stream buffer that transforms each line of the underlying input stream.
// If LineTransformer returns false the stream terminates with eof.
template <class LineTransformer>
class LineTransformBuf : public std::streambuf {
 public:
  LineTransformBuf(std::istream* input, LineTransformer transform)
      : input_(input), transform_(transform){};

  LineTransformBuf(const LineTransformBuf&) = delete;
  LineTransformBuf& operator=(const LineTransformBuf&) = delete;

  std::streambuf::int_type underflow() override {
    if (input_->eof()) {
      return traits_type::eof();
    }

    std::getline(*input_, line_);
    if (!transform_(&line_)) {
      return traits_type::eof();
    }

    if (!input_->eof()) {
      line_.push_back('\n');
    }

    setg(line_.data(), line_.data(), line_.data() + line_.size());
    return traits_type::to_int_type(*gptr());
  }

 private:
  std::istream* input_;
  std::string line_;
  LineTransformer transform_;
};

}  // namespace lhat::util

#endif  // LHAT_UTIL_LINE_TRANSFORM_BUF_H
