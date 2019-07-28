#ifndef marlin_parse_interpreter_hpp
#define marlin_parse_interpreter_hpp

#include <marlin/ast.hpp>

#include "scanner.hpp"

namespace marlin::parse {

struct interpreter {
  inline interpreter(scanner sc)
      : _sc{std::move(sc)},
        _current_loc{_sc.current_loc()},
        _current_token{_sc.scan()} {}

  inline code parse() { return parse_precedence(expression_base_precedence); }

 private:
  enum class precedence : uint8_t {
    none,
    term /* + - */,
    factor /* * / */,
    unary,
    call /* . () [] */,
    primary
  };

  static constexpr auto expression_base_precedence{precedence::term};

  struct infix_builder;

  scanner _sc;
  source_loc _current_loc;
  token _current_token;

  inline infix_builder make_builder(code node, uint8_t threshold_prec);

  inline code parse_precedence(precedence p) {
    return parse_precedence(static_cast<uint8_t>(p));
  }
  code parse_precedence(uint8_t p);

  inline utils::move_vector<code> parse_arguments() {
    next();
    utils::move_vector<code> args;
    while (true) {
      args.emplace_back(parse_precedence(expression_base_precedence));
      switch (_current_token.type) {
        case token_type::comma:
          next();
          break;
        case token_type::right_paren:
          next();
          return std::move(args);
        default:
          // TODO: handle error!
          break;
      }
    }
  }

  inline code parse_group() {
    next();
    auto group{parse_precedence(expression_base_precedence)};
    consume(token_type::right_paren);
    return std::move(group);
  }

  inline code parse_unary(ast::unary_op op) {
    next();
    return ast::unary_expression{op, parse_precedence(precedence::unary)};
  }

  inline code parse_identifier() {
    auto node = ast::identifier{{_current_token.start, _current_token.end}};
    next();
    return std::move(node);
  }

  inline code parse_number() {
    auto node = ast::number_literal{{_current_token.start, _current_token.end}};
    next();
    return std::move(node);
  }

  inline void consume(token_type type) {
    if (_current_token.type == type) {
      next();
    } else {
      // TODO: report error
      next();
    }
  }

  inline void next() {
    _current_loc = _sc.current_loc();
    _current_token = _sc.scan();
  }
};

}  // namespace marlin::parse

#endif  // marlin_parse_interpreter_hpp