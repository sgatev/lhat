#ifndef LHAT_CORE_PARSE_H
#define LHAT_CORE_PARSE_H

#include <istream>
#include <string>
#include <variant>

namespace lhat {
namespace core {
// Represents an error that has occurred during parsing.
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

// The result of parsing an expression. Could be either an object or an error if
// any occurred during parsing.
template <class T>
class ParseResult {
 public:
  ParseResult(T result) : result_(std::move(result)) {}
  ParseResult(ParseError error) : result_(std::move(error)) {}

  ~ParseResult() = default;
  ParseResult(const ParseResult& other) = default;
  ParseResult(ParseResult&& other) = default;
  ParseResult& operator=(const ParseResult& other) = default;
  ParseResult& operator=(ParseResult&& other) = default;

  // Returns the result of a successful parsing.
  const T& Value() const { return std::get<T>(result_); }

  // Releases the result of a successful parsing. The ParseResult should not be
  // used after invoking this method.
  T&& ReleaseValue() { return std::get<T>(result_); }

  // Returns the error that has occurred during parsing.
  ParseError Error() const { return std::get<ParseError>(result_); }

  // Returns true if no error has occurred during parsing.
  bool Ok() const { return std::holds_alternative<T>(result_); }

 private:
  std::variant<T, ParseError> result_;
};

// Discards all starting whitespaces from input.
void DiscardWhitespace(std::istream* input);
}  // namespace core
}  // namespace lhat

#endif  // LHAT_CORE_PARSE_H
