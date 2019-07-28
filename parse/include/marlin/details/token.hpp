#ifndef marlin_parse_token_hpp
#define marlin_parse_token_hpp

#include <string>

#include <marlin/ast.hpp>

namespace marlin::parse {

enum class token_type {
  left_paren,
  right_paren,
  comma,
  plus,
  minus,
  star,
  slash,
  identifier,
  number,
  eof
};

struct token {
  token_type type;
  std::string::const_iterator start;
  std::string::const_iterator end;
  source_loc start_loc;

  inline token(token_type _type, std::string::const_iterator _start,
               std::string::const_iterator _end, source_loc _start_loc) noexcept
      : type{_type}, start{_start}, end{_end}, start_loc{_start_loc} {}
};

}  // namespace marlin::parse

#endif  // marlin_parse_token_hpp