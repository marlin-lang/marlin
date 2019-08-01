#include "interpreter.hpp"

#include <unordered_map>

#include "infix_builder.hpp"

namespace marlin::parse {

code interpreter::parse_statement() {
  const auto start{_current_token.start};
  const auto start_ptr{_current_token.start_ptr};
  try {
    switch (_current_token.type) {
      // TODO: test for other statements
      default: {
        auto expression = parse_precedence(expression_base_precedence);
        consume(token_type::semicolon);
        return finalize_node(
            code::make<ast::expression_statement>(std::move(expression)),
            start);
      }
    }
  } catch (error e) {
    return parse_error(std::move(e), start, start_ptr);
  }
};

code interpreter::parse_precedence(uint8_t p) {
  const auto start{_current_token.start};
  code node{parse_expression_head()};

  auto builder{make_builder(std::move(node), p, start)};
  while (builder.success()) {
    switch (_current_token.type) {
      case token_type::left_paren:
        builder.parse_call();
        break;
      case token_type::plus:
        builder.parse_binary(ast::binary_op::add, precedence::term);
        break;
      case token_type::minus:
        builder.parse_binary(ast::binary_op::subtract, precedence::term);
        break;
      case token_type::star:
        builder.parse_binary(ast::binary_op::multiply, precedence::factor);
        break;
      case token_type::slash:
        builder.parse_binary(ast::binary_op::divide, precedence::factor);
        break;
      default:
        return std::move(builder).result();
    }
  }
  return std::move(builder).result();
}

}  // namespace marlin::parse
