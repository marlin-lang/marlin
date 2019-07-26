#ifndef marlin_parse_token_hpp
#define marlin_parse_token_hpp

#include <string>

namespace marlin::parse {

enum class token_type {
  left_paren,
  right_paren,
  plus,
  minus,
  star,
  slash,
  number,
  eof
};

struct token {
  token_type type;
  std::string::const_iterator start;
  std::string::const_iterator end;

  inline token(token_type _type, std::string::const_iterator _start,
               std::string::const_iterator _end) noexcept
      : type{_type}, start{_start}, end{_end} {}
};

}  // namespace marlin::parse

#endif  // marlin_parse_token_hpp