#ifndef marlin_parse_scanner_hpp
#define marlin_parse_scanner_hpp

#include <string>
#include <utility>

#include <marlin/ast.hpp>

#include "token.hpp"

namespace marlin::parse {

struct scanner {
  inline scanner(const std::string& source)
      : _source{source}, _current{_source.begin()}, _current_loc{1, 1} {}

  inline source_loc current_loc() { return _current_loc; }

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
          _current_loc = {_current_loc.line + 1, 1};
          _current++;
          break;
        default:
          return;
      }
    }
  }

  inline void consume_identifier() {
    while (_current < _source.end() &&
           (is_id_head(*_current) || is_digit(*_current))) {
      advance();
    }
  }

  inline void consume_number() {
    while (_current < _source.end() && is_digit(*_current)) {
      advance();
    }
    if (_current + 1 < _source.end() && *_current == '.' &&
        is_digit(*(_current + 1))) {
      advance();
      while (_current < _source.end() && is_digit(*_current)) {
        advance();
      }
    }
  }

  inline char advance() {
    const auto ch{*_current};
    _current_loc.column++;
    _current++;
    return ch;
  }

  inline bool is_id_head(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' ||
           ch == '$';
  }
  inline bool is_digit(char ch) { return ch >= '0' && ch <= '9'; }

};  // namespace marlin::parse

}  // namespace marlin::parse

#endif  // marlin_parse_scanner_hpp