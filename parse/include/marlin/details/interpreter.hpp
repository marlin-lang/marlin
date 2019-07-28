#ifndef marlin_parse_interpreter_hpp
#define marlin_parse_interpreter_hpp

#include <marlin/ast.hpp>

#include "scanner.hpp"

namespace marlin::parse {

struct interpreter {
  inline interpreter(scanner sc)
      : _sc{std::move(sc)},
        _previous_token_end{_sc.current_loc()},
        _current_token{_sc.scan()} {}

  inline code parse() {
    const auto start{_current_token.start_loc};
    utils::move_vector<code> statements;
    while (_current_token.type != token_type::eof) {
      statements.emplace_back(parse_statement());
    }
    return with_range(ast::program{std::move(statements)}, start);
  }

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
  source_loc _previous_token_end;
  token _current_token;

  inline infix_builder make_builder(code node, uint8_t threshold_prec,
                                    source_loc start);

  inline code with_range(code c, source_loc start) {
    c.get()._source_range = {start, _previous_token_end};
    return c;
  }

  code parse_statement();

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
          return args;
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
    return group;
  }

  inline code parse_unary(ast::unary_op op) {
    const auto start{_current_token.start_loc};
    next();
    return with_range(
        ast::unary_expression{op, parse_precedence(precedence::unary)}, start);
  }

  inline code parse_identifier() {
    const auto start{_current_token.start_loc};
    auto node = ast::identifier{{_current_token.start, _current_token.end}};
    next();
    return with_range(std::move(node), start);
  }

  inline code parse_number() {
    const auto start{_current_token.start_loc};
    auto node = ast::number_literal{{_current_token.start, _current_token.end}};
    next();
    return with_range(std::move(node), start);
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
    _previous_token_end = _sc.current_loc();
    _current_token = _sc.scan();
  }
};

}  // namespace marlin::parse

#endif  // marlin_parse_interpreter_hpp