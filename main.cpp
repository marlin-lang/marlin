#include <iostream>

#include <marlin/exec.hpp>
#include <marlin/parse.hpp>

int main(int argc, char* argv[]) {
  std::cout << "marlin-cli work in progross...\n";

  std::cout << "\nTesting:\n";
  marlin::exec::environment env;
  env.register_print_callback([](auto string) { std::cout << string; });

  auto code = marlin::parse::process("10 + -7 / (5 - 3)");
  env.execute(code);

  return 0;
}