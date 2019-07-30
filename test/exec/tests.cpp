#include <catch2/catch.hpp>

#include <marlin/exec.hpp>
#include <marlin/lint.hpp>
#include <marlin/parse.hpp>

TEST_CASE("exec::Execute print statements", "[exec]") {
  const std::string script{
      "print(-6.5 * -5 / -6 + 8 + 7 * (12 / (11 + 14.2)) / 5);\n"
      "print(\" \");\n"
      "print(true);\n"
      "print(\" \");\n"
      "print(false);\n"
      "print(\" \");\n"
      "print(1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 + 10);\n"
      "print(\"\\n\");\n"};

  auto [code, parse_errors]{marlin::parse::process(script)};
  REQUIRE(parse_errors.size() == 0);

  marlin::lint::linter l{code};
  l.lint();

  std::string result;
  marlin::exec::environment env;
  env.register_print_callback(
      [&result](const auto &string) { result.append(string); });
  REQUIRE_NOTHROW(env.execute(code));
  REQUIRE(result == "3.25 true false 55\n");
}