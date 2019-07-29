#ifndef marlin_exec_generator_hpp
#define marlin_exec_generator_hpp

#include <type_traits>

#include <jsast/jsast.hpp>

#include <marlin/ast.hpp>

#include "errors.hpp"

namespace marlin::exec {

struct generator {
  static inline std::string generate(code& c) {
    jsast::generator gen;
    gen.write(get_node(c));
    return std::move(gen).str();
  }

 private:
  static inline jsast::ast::node get_node(code& c) {
    return c.apply<jsast::ast::node>([](auto& node) { return get_node(node); });
  }

  template <typename node_type, typename = std::enable_if_t<
                                    std::is_base_of_v<ast::base, node_type>>>
  static inline jsast::ast::node get_node(node_type& node) {
    return {get_jsast(node),
            [&node](source_range range) { node._js_range = range; }};
  }

  // Unused return value type, specified so that the code compiles
  [[noreturn]] static inline jsast::ast::empty_statement get_jsast(
      ast::erroneous_line& node) {
    throw generation_error{"Unparsed chunk encountered!", node};
  }

  static inline auto get_jsast(ast::program& program) {
    utils::move_vector<jsast::ast::node> statements;
    for (size_t i{0}; i < program.statement_count(); i++) {
      statements.emplace_back(get_node(program.statement(i)));
    }
    return jsast::ast::program{std::move(statements)};
  }

  static inline auto get_jsast(ast::expression_statement& statement) {
    return jsast::ast::expression_statement{get_node(statement.expression())};
  }

  static inline auto get_jsast(ast::unary_expression& unary) {
    static constexpr jsast::unary_op symbol_map[]{
        jsast::unary_op::positive /* positive */,
        jsast::unary_op::negative /* negative */
    };
    return jsast::ast::unary_expression{
        symbol_map[static_cast<uint8_t>(unary.op)], get_node(unary.argument())};
  }

  static inline auto get_jsast(ast::binary_expression& binary) {
    static constexpr jsast::binary_op symbol_map[]{
        jsast::binary_op::add /* add */,
        jsast::binary_op::subtract /* subtract */,
        jsast::binary_op::multiply /* multiply */,
        jsast::binary_op::divide /* divide */
    };
    return jsast::ast::binary_expression{
        get_node(binary.left()), symbol_map[static_cast<uint8_t>(binary.op)],
        get_node(binary.right())};
  }

  static inline auto get_jsast(ast::call_expression& call) {
    utils::move_vector<jsast::ast::node> arguments;
    for (size_t i{0}; i < call.argument_count(); i++) {
      arguments.emplace_back(get_node(call.argument(i)));
    }
    return jsast::ast::call_expression{get_node(call.callee()),
                                       std::move(arguments)};
  }

  static inline auto get_jsast(ast::identifier& identifier) {
    return jsast::ast::identifier{identifier.name};
  }

  static inline auto get_jsast(ast::bool_literal& literal) {
    return jsast::ast::bool_literal{literal.value};
  }

  static inline auto get_jsast(ast::number_literal& literal) {
    return jsast::ast::raw_literal{literal.number};
  }

  static inline auto get_jsast(ast::string_literal& literal) {
    return jsast::ast::string_literal{literal.string};
  }

};  // namespace marlin::exec

}  // namespace marlin::exec

#endif  // marlin_exec_generator_hpp