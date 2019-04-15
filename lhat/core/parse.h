#ifndef LHAT_CORE_PARSE_H
#define LHAT_CORE_PARSE_H

#include <string>
#include <variant>

namespace lhat {
namespace core {
// ParseError represents an error that has occurred during parsing.
class ParseError {
public:
  explicit ParseError(std::string message) : message_(std::move(message)) {}

  ~ParseError() = default;
  ParseError(const ParseError& other) = default;
  ParseError(ParseError&& other) = default;
  ParseError& operator=(const ParseError& other) = default;
  ParseError& operator=(ParseError&& other) = default;

  // Message returns the error message.
  std::string Message() const { return message_; }

private:
  std::string message_;
};

// ParseResult is the result of parsing an expression. It is either an object or
// an error if any occurred.
template <class T>
class ParseResult {
public:
  explicit ParseResult(int consumed_chars_count, T result)
      : consumed_chars_count_(consumed_chars_count),
        result_(std::move(result)) {}

  explicit ParseResult(int consumed_chars_count, ParseError error)
      : consumed_chars_count_(consumed_chars_count),
        result_(std::move(error)) {}

  ~ParseResult() = default;
  ParseResult(const ParseResult& other) = default;
  ParseResult(ParseResult&& other) = default;
  ParseResult& operator=(const ParseResult& other) = default;
  ParseResult& operator=(ParseResult&& other) = default;

  // ConsumedChars returns the number of characters consumed from the expression
  // during parsing.
  int ConsumedChars() const { return consumed_chars_count_; }

  // Value returns the result of a successful parsing.
  const T& Value() const { return std::get<T>(result_); }

  // ReleaseValue releases the result of a successful parsing. The ParseResult
  // should not be used after invoking this method.
  T&& ReleaseValue() { return std::get<T>(result_); }

  // Error returns the error that has occurred during parsing.
  ParseError Error() const { return std::get<ParseError>(result_); }

  // Ok returns true if no error has occurred during parsing.
  bool Ok() const { return std::holds_alternative<T>(result_); }

private:
  int consumed_chars_count_;
  std::variant<T, ParseError> result_;
};
}  // namespace core
}  // namespace lhat

#endif  // LHAT_CORE_PARSE_H
