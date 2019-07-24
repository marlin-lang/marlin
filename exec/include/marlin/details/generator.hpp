#ifndef marlin_exec_generator_hpp
#define marlin_exec_generator_hpp

#include <unordered_map>

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
    return gen.str();
  }

 private:
  template <typename node_type>
  static inline auto set_range(node_type& node) {
    return [&node](source_range range) { node._js_range = range; };
  }

  static inline jsast::ast::node get_node(code& c) {
    return c.apply<jsast::ast::node>([](auto& node) { return get_node(node); });
  }

  static inline jsast::ast::node get_node(ast::number_literal& literal) {
    return {jsast::ast::number_literal{literal.number}, set_range(literal)};
  }

  static inline jsast::ast::node get_node(ast::binary_expression& binary) {
    static std::unordered_map<ast::binary_op, jsast::binary_op> symbol_map{
        {ast::binary_op::add, jsast::binary_op::add},
        {ast::binary_op::subtract, jsast::binary_op::subtract},
        {ast::binary_op::multiply, jsast::binary_op::multiply},
        {ast::binary_op::divide, jsast::binary_op::divide}};
    return {jsast::ast::binary_expression{get_node(binary.left()),
                                          symbol_map[binary.op],
                                          get_node(binary.right())},
            set_range(binary)};
  }

};  // namespace marlin::exec

}  // namespace marlin::exec

#endif  // marlin_exec_generator_hpp