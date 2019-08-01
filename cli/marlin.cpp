#include <iostream>

#include <marlin/exec.hpp>
#include <marlin/format.hpp>
#include <marlin/lint.hpp>
#include <marlin/parse.hpp>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    try {
      auto [code, parse_errors]{marlin::parse::process_file(argv[1])};
      if (parse_errors.size() > 0) {
        for (const auto &e : parse_errors) {
          std::cout << argv[1] << ":" << e.loc().line << ":" << e.loc().column
                    << ": " << e.what() << "\n";
        }
        std::cout << parse_errors.size() << " parsing errors generated.\n";
        return 3;
      } else {
        marlin::lint::linter l{code.get()};
        l.lint();

        marlin::exec::environment env;
        env.register_print_callback(
            [](const auto &string) { std::cout << string; });
        try {
          env.execute(code.get(), argv[1]);
          return 0;
        } catch (const marlin::exec::generation_error &e) {
          const auto loc{e.node().source_code_range().begin};
          std::cout << argv[1] << ":" << loc.line << ":" << loc.column << ": "
                    << "Internal Error: " << e.what() << "\n";
          return 5;
        } catch (const marlin::exec::runtime_error &e) {
          std::cout << e.what() << "\n";
          return 5;
        }
      }
    } catch (const marlin::parse::filesystem_error &e) {
      std::cout << e.what() << "\n";
      return 2;
    }
  } else {
    std::cout << "marlin does not support interactive execution for now!\n"
                 "Please use:\n\n"
                 "    marlin <filename>\n\n";
    return 1;
  }
}
