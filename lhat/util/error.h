#ifndef LHAT_UTIL_ERROR_H
#define LHAT_UTIL_ERROR_H

#include <string>
#include <variant>

namespace lhat {
namespace util {
// Represents a generic error.
class Error {
 public:
  explicit Error(std::string message) : message_(std::move(message)) {}

  ~Error() = default;
  Error(const Error& other) = default;
  Error(Error&& other) = default;
  Error& operator=(const Error& other) = default;
  Error& operator=(Error&& other) = default;

  // Returns the error message.
  std::string Message() const { return message_; }

 private:
  std::string message_;
};

// Container of a value or an error.
template <class T>
class ErrorOr {
 public:
  ErrorOr(T result) : result_(std::move(result)) {}
  ErrorOr(util::Error error) : result_(std::move(error)) {}

  ~ErrorOr() = default;
  ErrorOr(const ErrorOr& other) = default;
  ErrorOr(ErrorOr&& other) = default;
  ErrorOr& operator=(const ErrorOr& other) = default;
  ErrorOr& operator=(ErrorOr&& other) = default;

  // Returns the contained value.
  const T& Value() const { return std::get<T>(result_); }

  // Returns the contained error.
  const util::Error& Error() const { return std::get<util::Error>(result_); }

  // Returns true if the container contains a value.
  bool Ok() const { return std::holds_alternative<T>(result_); }

 private:
  std::variant<T, util::Error> result_;
};
}  // namespace util
}  // namespace lhat

#endif  // LHAT_UTIL_ERROR_H
