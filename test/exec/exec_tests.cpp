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

  marlin::lint::linter l{code.get()};
  l.lint();

  std::string result;
  marlin::exec::environment env;
  env.register_print_callback(
      [&result](const auto &string) { result.append(string); });
  REQUIRE_NOTHROW(env.execute(code.get()));
  REQUIRE(result == "3.25 true false 55\n");
}

TEST_CASE("exec::Evaluate variable declarations", "[exec]") {
  const std::string script{
      "let a = 1;\n"
      "print(a);\n"};

  auto [code, parse_errors]{marlin::parse::process(script)};
  REQUIRE(parse_errors.size() == 0);

  marlin::lint::linter l{code.get()};
  l.lint();

  std::string result;
  marlin::exec::environment env;
  env.register_print_callback(
      [&result](const auto &string) { result.append(string); });
  REQUIRE_NOTHROW(env.execute(code.get()));
  REQUIRE(result == "1");
}

// TODO: after implementing type checking, this will become lint error
TEST_CASE("exec::Report runtime error", "[exec]") {
  const std::string script{"print(a);\n"};

  auto [code, parse_errors]{marlin::parse::process(script)};
  REQUIRE(parse_errors.size() == 0);

  marlin::lint::linter l{code.get()};
  l.lint();

  std::string result;
  marlin::exec::environment env;
  env.register_print_callback(
      [&result](const auto &string) { result.append(string); });

  bool thrown{false};
  try {
    env.execute(code.get());
  } catch (const marlin::exec::runtime_error &error) {
    thrown = true;
    CHECK(error.stack_depth() == 1);
    CHECK(error.stack(0).is<marlin::ast::identifier>());
    CHECK(&error.stack(0) ==
          &code->children()[0]->children()[0]->children()[1].get());
  }
  REQUIRE(thrown);
}
