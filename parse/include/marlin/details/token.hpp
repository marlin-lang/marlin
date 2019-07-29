#ifndef marlin_parse_token_hpp
#define marlin_parse_token_hpp

#include <optional>
#include <string>

#include <marlin/ast.hpp>

namespace marlin::parse {

enum class token_type : uint8_t {
  left_paren,
  right_paren,
  dot,
  comma,
  semicolon,
  plus,
  minus,
  star,
  slash,
  kwd_true,
  kwd_false,
  identifier,
  number,
  string,
  eof
};

static constexpr const char* _token_type_name_map[]{
    "\"(\"" /* left_paren */,
    "\")\"" /* right_paren */,
    "\".\"" /* dot */,
    "\",\"" /* comma */,
    "\";\"" /* semicolon */,
    "\"+\"" /* plus */,
    "\"-\"" /* minus */,
    "\"*\"" /* star */,
    "\"/\"" /* slash */,
    "\"true\"" /* kwd_true */,
    "\"false\"" /* kwd_false */,
    "(id)" /* identifier */,
    "(number)" /* number */,
    "(string)" /* string */,
    "end of file" /* eof */
};
inline constexpr const char* name_for(token_type type) noexcept {
  return _token_type_name_map[static_cast<uint8_t>(type)];
}

struct token {
  token_type type;
  source_loc start;
  std::string::const_iterator start_ptr;
  std::optional<code> parsed_node;

  inline token(token_type _type, source_loc _start,
               std::string::const_iterator _start_ptr,
               std::optional<code> _node = std::nullopt) noexcept
      : type{_type},
        start{_start},
        start_ptr{_start_ptr},
        parsed_node{std::move(_node)} {}

 private:
  // filled when type is identifier, number
};

}  // namespace marlin::parse

#endif  // marlin_parse_token_hpp