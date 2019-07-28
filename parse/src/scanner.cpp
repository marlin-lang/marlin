#include "scanner.hpp"

namespace marlin::parse {

token scanner::scan() {
  skip_whitespace();

  const auto make_token = [this, start{_current},
                           start_loc{_current_loc}](token_type type) {
    return token{type, start, _current, start_loc};
  };
  if (_current >= _source.end()) {
    return make_token(token_type::eof);
  } else {
    const auto ch{advance()};
    switch (ch) {
      case '(':
        return make_token(token_type::left_paren);
      case ')':
        return make_token(token_type::right_paren);
      case '.':
        if (_current < _source.end() && is_digit(*_current)) {
          consume_number();
          return make_token(token_type::number);
        } else {
          return make_token(token_type::dot);
        }
      case ',':
        return make_token(token_type::comma);
      case ';':
        return make_token(token_type::semicolon);
      case '+':
        return make_token(token_type::plus);
      case '-':
        return make_token(token_type::minus);
      case '*':
        return make_token(token_type::star);
      case '/':
        return make_token(token_type::slash);
      default:
        if (is_id_head(ch)) {
          consume_identifier();
          return make_token(token_type::identifier);
        } else if (is_digit(ch)) {
          consume_number();
          return make_token(token_type::number);
        } else {
          // TODO: raise error
          return make_token(token_type::eof);
        }
    }
  }
}

}  // namespace marlin::parse