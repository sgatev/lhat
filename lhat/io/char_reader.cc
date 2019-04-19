#include "lhat/io/char_reader.h"

namespace lhat {
namespace io {
CharReader::CharReader(std::istream* stream) : stream_(stream), idx_(0) {
  std::getline(*stream_, line_);
}

void CharReader::Next() {
  if (idx_ < line_.size()) {
    idx_++;
  } else {
    std::getline(*stream_, line_);
    idx_ = 0;
  }
}

char CharReader::Peek() const { return line_[idx_]; }

bool CharReader::Empty() const {
  return idx_ >= line_.size() && stream_->eof();
}
}  // namespace io
}  // namespace lhat
