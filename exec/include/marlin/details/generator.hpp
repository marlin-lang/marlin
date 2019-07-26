#ifndef marlin_exec_generator_hpp
#define marlin_exec_generator_hpp

#include <type_traits>

#include <jsast/jsast.hpp>

#include <marlin/ast.hpp>

namespace marlin::exec {

struct generator {
  static inline std::string generate(code& c) {
    jsast::generator gen;
    gen.write(jsast::ast::program{
        {jsast::ast::expression_statement{jsast::ast::call_expression{
             jsast::ast::identifier{"print"}, {get_node(c)}}},
         jsast::ast::expression_statement{jsast::ast::call_expression{
             jsast::ast::identifier{"print"},
             {jsast::ast::string_literal("\n")}}}}});
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

  static inline auto get_jsast(ast::number_literal& literal) {
    return jsast::ast::raw_literal{literal.number};
  }

};  // namespace marlin::exec

}  // namespace marlin::exec

#endif  // marlin_exec_generator_hpp