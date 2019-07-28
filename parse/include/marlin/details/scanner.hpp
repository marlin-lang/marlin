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
          consume_new_line();
          break;
        default:
          return;
      }
    }
  }

  inline token make_bare_token(token_type type) {
    const auto start{_current_loc};
    advance();
    return {type, start};
  }

  inline token make_identifier_or_keyword_token() {
    const auto start{_current_loc};
    const auto start_iter{_current};
    advance();
    while (_current < _source.end() &&
           (is_id_head(*_current) || is_digit(*_current))) {
      advance();
    }

    // TODO:: check for keyword

    return {token_type::identifier, start,
            ast::identifier{{start_iter, _current}}};
  }

  inline token make_number_token() {
    const auto start{_current_loc};
    const auto start_iter{_current};
    advance();
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
    return {token_type::number, start,
            ast::number_literal{{start_iter, _current}}};
  }

  inline token make_string_token() {
    const auto start{_current_loc};
    const char delim{*_current};
    std::string result;
    advance();
    bool success{false}, escaped{false};
    while (_current < _source.end() && !success) {
      if (escaped) {
        escaped = false;
        switch (*_current) {
          case 'n':
            result.push_back('\n');
            break;
          case 't':
            result.push_back('\t');
            break;
          default:
            result.push_back(*_current);
            break;
        }
      } else if (*_current == '\\') {
        escaped = true;
      } else if (*_current == delim) {
        success = true;
      } else {
        result.push_back(*_current);
      }

      if (*_current == '\n') {
        consume_new_line();
      } else {
        advance();
      }
    }
    // handling errors
    return {token_type::string, start, ast::string_literal{std::move(result)}};
  }

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