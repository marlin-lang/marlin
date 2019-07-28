#include "scanner.hpp"

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
          // TODO: raise error
          return make_bare_token(token_type::eof);
        }
    }
  }
}

}  // namespace marlin::parse