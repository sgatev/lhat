#ifndef LHAT_IO_CHAR_READER_H
#define LHAT_IO_CHAR_READER_H

#include <istream>
#include <string>

namespace lhat {
namespace io {
// Reads an input stream one character at a time.
class CharReader {
 public:
  CharReader(std::istream* stream);

  // Advance the reader with one char.
  void Next();

  // Returns the current char.
  char Peek() const;

  // Returns true if there are no more chars to be read.
  bool Empty() const;

 private:
  std::istream* stream_;
  std::string line_;
  int idx_;
};
}  // namespace io
}  // namespace lhat

#endif  // LHAT_IO_CHAR_READER_H
