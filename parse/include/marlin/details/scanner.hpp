#ifndef marlin_parse_scanner_hpp
#define marlin_parse_scanner_hpp

#include <string>

#include <marlin/ast.hpp>

#include "token.hpp"

// Test:
#include <iostream>

namespace marlin::parse {

struct scanner {
  inline scanner(const std::string& source)
      : _source{source}, _current{_source.begin()}, _current_loc{1, 1} {}

  inline source_loc current_loc() const noexcept { return _current_loc; }
  inline std::string::const_iterator current_ptr() const noexcept {
    return _current;
  }

  token scan();

 private:
  const std::string& _source;
  std::string::const_iterator _current;
  source_loc _current_loc;

  inline void skip_whitespace() {
    while (_current < _source.end()) {
      switch (*_current) {
        case ' ':
          [[fallthrough]];
        case '\r':
          [[fallthrough]];
        case '\t':
          advance();
          break;
        case '\n':
          consume_new_line();
          break;
        default:
          return;
      }
    }
  }

  inline token make_bare_token(token_type type) {
    const auto start{_current_loc};
    const auto start_iter{_current};
    advance();
    return {type, start, start_iter};
  }

  token make_identifier_or_keyword_token();
  token make_number_token();
  token make_string_token();

  inline void advance() {
    _current++;
    _current_loc.column++;
  }

  inline void consume_new_line() {
    _current++;
    _current_loc = {_current_loc.line + 1, 1};
  }

  inline bool is_id_head(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' ||
           ch == '$';
  }
  inline bool is_digit(char ch) { return ch >= '0' && ch <= '9'; }

};  // namespace marlin::parse

}  // namespace marlin::parse

#endif  // marlin_parse_scanner_hpp