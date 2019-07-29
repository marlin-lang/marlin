#include "scanner.hpp"

#include <cstring>

namespace marlin::parse {

token scanner::scan() {
  skip_whitespace();

  if (_current >= _source.end()) {
    return make_bare_token(token_type::eof);
  } else {
    switch (*_current) {
      case '(':
        return make_bare_token(token_type::left_paren);
      case ')':
        return make_bare_token(token_type::right_paren);
      case '.':
        if (_current + 1 < _source.end() && is_digit(*(_current + 1))) {
          return make_number_token();
        } else {
          return make_bare_token(token_type::dot);
        }
      case ',':
        return make_bare_token(token_type::comma);
      case ';':
        return make_bare_token(token_type::semicolon);
      case '+':
        return make_bare_token(token_type::plus);
      case '-':
        return make_bare_token(token_type::minus);
      case '*':
        return make_bare_token(token_type::star);
      case '/':
        return make_bare_token(token_type::slash);
      case '\'':
        [[fallthrough]];
      case '"':
        return make_string_token();
      default:
        if (is_id_head(*_current)) {
          return make_identifier_or_keyword_token();
        } else if (is_digit(*_current)) {
          return make_number_token();
        } else {
          return make_bare_token(token_type::error);
        }
    }
  }
}

token scanner::make_identifier_or_keyword_token() {
  const auto start{_current_loc};
  const auto start_iter{_current};
  advance();
  while (_current < _source.end() &&
         (is_id_head(*_current) || is_digit(*_current))) {
    advance();
  }

  const auto test_for = [start, start_iter, end_iter{_current}](
                            size_t offset, std::string str,
                            token_type keyword) {
    if (end_iter == start_iter + offset + str.size() &&
        memcmp(&*(start_iter + offset), str.data(), str.size()) == 0) {
      return token{keyword, start, start_iter};
    } else {
      return token{token_type::identifier, start, start_iter,
                   ast::identifier{{start_iter, end_iter}}};
    }
  };

  if (_current - start_iter > 0) {
    switch (*start_iter) {
      case 'f':
        return test_for(1, "alse", token_type::kwd_false);
      case 't':
        return test_for(1, "rue", token_type::kwd_true);
    }
  }
  return {token_type::identifier, start, start_iter,
          ast::identifier{{start_iter, _current}}};
}

token scanner::make_number_token() {
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
  return {token_type::number, start, start_iter,
          ast::number_literal{{start_iter, _current}}};
}

token scanner::make_string_token() {
  const auto start{_current_loc};
  const auto start_iter{_current};
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
  return {token_type::string, start, start_iter,
          ast::string_literal{std::move(result)}};
}

}  // namespace marlin::parse