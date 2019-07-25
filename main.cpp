#include <iostream>

#include <marlin/ast.hpp>
#include <marlin/exec.hpp>

int main(int argc, char* argv[]) {
  std::cout << "marlin-cli work in progross...\n";

  std::cout << "\nTesting:\n";
  marlin::exec::environment env;
  env.register_print_callback([](auto string) { std::cout << string; });

  marlin::code code = marlin::ast::binary_expression{
      marlin::ast::unary_expression{marlin::ast::unary_op::negative,
                                    marlin::ast::number_literal{10}},
      marlin::ast::binary_op::multiply, marlin::ast::number_literal{7}};
  env.execute(code);

  return 0;
}