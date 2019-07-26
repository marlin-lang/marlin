#include "scanner.hpp"

namespace marlin::parse {

token scanner::scan() {
  skip_whitespace();

  const auto make_token = [this, start{_current}](token_type type) {
    return token{type, start, _current};
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
      case '+':
        return make_token(token_type::plus);
      case '-':
        return make_token(token_type::minus);
      case '*':
        return make_token(token_type::star);
      case '/':
        return make_token(token_type::slash);
      case '0' ... '9':
        consume_number();
        return make_token(token_type::number);

      default:
        // TODO: raise error
        return make_token(token_type::eof);
    }
  }
}

}  // namespace marlin::parse