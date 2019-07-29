#ifndef marlin_parse_interpreter_hpp
#define marlin_parse_interpreter_hpp

#include <utility>

#include <marlin/ast.hpp>

#include "errors.hpp"
#include "scanner.hpp"

// Testing
#include <iostream>

namespace marlin::parse {

struct interpreter {
  inline interpreter(scanner sc)
      : _sc{std::move(sc)},
        _previous_token_end{_sc.current_loc()},
        _current_token{_sc.scan()} {}

  inline std::pair<code, std::vector<error>> parse() {
    const auto start{_current_token.start};
    std::pair<code, std::vector<error>> result{
        with_range(ast::program{parse_statements(token_type::eof)}, start),
        std::move(_errors)};
    _errors = {};
    return result;
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
  std::vector<error> _errors;

  inline infix_builder make_builder(code node, uint8_t threshold_prec,
                                    source_loc start);

  inline code with_range(code c, source_loc start) {
    c.get()._source_range = {start, _previous_token_end};
    return c;
  }

  inline void synchronize() {
    while (true) {
      switch (_current_token.type) {
        case token_type::semicolon:
          next();
          [[fallthrough]];
        case token_type::eof:
          return;

        default:
          next();
          break;
      }
    }
  }
  inline void synchronize_and_throw[[noreturn]](error e) {
    synchronize();
    throw e;
  }

  inline code parse_error(error e, source_loc start,
                          std::string::const_iterator start_ptr) {
    code node = ast::erroneous_line{{start_ptr, _sc.current_ptr()}};
    e.set_node(node.get());
    _errors.push_back(std::move(e));
    return with_range(std::move(node), start);
  }

  inline utils::move_vector<code> parse_statements(token_type terminator) {
    utils::move_vector<code> statements;
    while (_current_token.type != token_type::eof &&
           _current_token.type != terminator) {
      statements.emplace_back(parse_statement());
    }
    if (_current_token.type == token_type::eof &&
        terminator != token_type::eof) {
      throw error{std::string{"Unexpected "} + name_for(token_type::eof) +
                      ", expecting " + name_for(terminator) + ".",
                  _current_token.start};
    } else {
      return statements;
    }
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
          synchronize_and_throw(
              {std::string{"Unexpected "} + name_for(_current_token.type) + ".",
               _current_token.start});
      }
    }
  }

  inline code parse_group() {
    next();
    auto group{parse_precedence(expression_base_precedence)};
    consume(token_type::right_paren);
    return group;
  }

  inline code parse_expression_head() {
    switch (_current_token.type) {
      case token_type::left_paren:
        return parse_group();
      case token_type::plus:
        return parse_unary(ast::unary_op::positive);
      case token_type::minus:
        return parse_unary(ast::unary_op::negative);
      case token_type::kwd_true:
        return bool_literal(true);
      case token_type::kwd_false:
        return bool_literal(false);
      case token_type::identifier:
        [[fallthrough]];
      case token_type::number:
        [[fallthrough]];
      case token_type::string:
        return parse_standalone_token();
      default:
        synchronize_and_throw({std::string{"Unexpected "} +
                                   name_for(_current_token.type) +
                                   ", expecting expression.",
                               _current_token.start});
    }
  }

  inline code parse_unary(ast::unary_op op) {
    const auto start{_current_token.start};
    next();
    return with_range(
        ast::unary_expression{op, parse_precedence(precedence::unary)}, start);
  }

  inline code parse_standalone_token() {
    const auto start{_current_token.start};
    if (_current_token.parsed_node.has_value()) {
      auto node{std::move(_current_token.parsed_node).value()};
      next();
      return with_range(std::move(node), start);
    } else {
      synchronize_and_throw({std::string{"Internal error when parsing "} +
                                 name_for(_current_token.type) + ".",
                             _current_token.start});
    }
  }

  inline code bool_literal(bool value) {
    const auto start{_current_token.start};
    next();
    return with_range(ast::bool_literal{value}, start);
  }

  inline void consume(token_type type) {
    if (_current_token.type == type) {
      next();
    } else {
      error e{std::string{"Expecting "} + name_for(type) + ".",
              _current_token.start};
      // TODO: test for right_brace when implementing blocks
      if (type != token_type::semicolon) {
        synchronize();
      }
      throw e;
    }
  }

  inline void next() {
    _previous_token_end = _sc.current_loc();
    _current_token = _sc.scan();
  }
};

}  // namespace marlin::parse

#endif  // marlin_parse_interpreter_hpp