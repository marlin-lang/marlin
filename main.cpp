#include <iostream>

#include <marlin/exec.hpp>
#include <marlin/parse.hpp>

int main(int argc, char* argv[]) {
  if (argc > 1) {
    marlin::exec::environment env;
    env.register_print_callback([](auto string) { std::cout << string; });
    auto code = marlin::parse::process_file(argv[1]);
    env.execute(code, argv[1]);
    return 0;
  } else {
    std::cout << "marlin does not support interactive execution for now!\n"
                 "Please use:\n\n"
                 "    marlin <filename>\n\n";
    return 1;
  }
}