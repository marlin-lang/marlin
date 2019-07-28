#ifndef marlin_parse_token_hpp
#define marlin_parse_token_hpp

#include <optional>
#include <string>

#include <marlin/ast.hpp>

namespace marlin::parse {

enum class token_type {
  left_paren,
  right_paren,
  dot,
  comma,
  semicolon,
  plus,
  minus,
  star,
  slash,
  identifier,
  number,
  string,
  eof
};

struct token {
  token_type type;
  source_loc start;

  inline token(token_type _type, source_loc _start,
               std::optional<code> _node = std::nullopt) noexcept
      : type{_type}, start{_start}, _parsed_node{std::move(_node)} {}

  inline code parsed() && { return std::move(_parsed_node).value(); }

 private:
  // filled when type is identifier, number
  std::optional<code> _parsed_node;
};

}  // namespace marlin::parse

#endif  // marlin_parse_token_hpp