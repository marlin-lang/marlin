#include "interpreter.hpp"

#include <unordered_map>

#include "infix_builder.hpp"

namespace marlin::parse {

code interpreter::parse_precedence(uint8_t p) {
  code node{[this]() {
    switch (_current_token.type) {
      case token_type::left_paren:
        return parse_group();
      case token_type::plus:
        return parse_unary(ast::unary_op::positive);
      case token_type::minus:
        return parse_unary(ast::unary_op::negative);
      case token_type::identifier:
        return parse_identifier();
      case token_type::number:
        return parse_number();
      default:
        // TODO: handle error
        return code{ast::number_literal{"0"}};
    }
  }()};

  auto builder{make_builder(std::move(node), p)};
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
        // TODO: handle error
        return std::move(builder).result();
    }
  }
  return std::move(builder).result();
}

}  // namespace marlin::parse