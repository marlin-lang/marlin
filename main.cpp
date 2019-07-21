#include <iostream>

#include <marlin/exec.hpp>

int main(int argc, char* argv[]) {
  std::cout << "marlin-cli work in progross...\n";

  std::cout << "\nTesting:\n";
  marlin::exec::environment env;
  env.register_print_callback([](auto string) { std::cout << string; });
  env.execute("print(\"Hello, world!\\n\")");

  return 0;
}