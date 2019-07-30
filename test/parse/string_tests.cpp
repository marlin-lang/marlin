#include <catch2/catch.hpp>

#include <optional>
#include <string>

#include <marlin/parse.hpp>

const std::string* find_string_literal(const marlin::code& c) {
  if (c->children_count() == 1 && c->child(0)->children_count() == 1 &&
      c->child(0)->child(0).is<marlin::ast::string_literal>()) {
    return &c->child(0)->child(0).as<marlin::ast::string_literal>().string;
  } else {
    return nullptr;
  }
}

TEST_CASE("parse::Parse double-quoted strings", "[parse]") {
  const auto [code, errors] = marlin::parse::process("\"a\";");
  REQUIRE(errors.size() == 0);

  const auto* string = find_string_literal(code);
  REQUIRE(string != nullptr);
  REQUIRE(*string == "a");
}

TEST_CASE("parse::Parse single-quoted strings", "[parse]") {
  const auto [code, errors] = marlin::parse::process("'a';");
  REQUIRE(errors.size() == 0);

  const auto* string = find_string_literal(code);
  REQUIRE(string != nullptr);
  REQUIRE(*string == "a");
}

TEST_CASE("parse::Parse double-quoted escaped characters", "[parse]") {
  const auto [code, errors] = marlin::parse::process("\"\\\\\\n'\\\"\";");
  REQUIRE(errors.size() == 0);

  const auto& string = find_string_literal(code);
  REQUIRE(string != nullptr);
  REQUIRE(*string == "\\\n'\"");
}

TEST_CASE("parse::Parse single-duoted escaped characters", "[parse]") {
  const auto [code, errors] = marlin::parse::process("'\\\\\\n\\'\"';");
  REQUIRE(errors.size() == 0);

  const auto& string = find_string_literal(code);
  REQUIRE(string != nullptr);
  REQUIRE(*string == "\\\n'\"");
}

TEST_CASE("parse::Parse multi-line strings", "[parse]") {
  const auto [code, errors] = marlin::parse::process("\"a\nb\\\nc\";");
  REQUIRE(errors.size() == 0);

  const auto& string = find_string_literal(code);
  REQUIRE(string != nullptr);
  REQUIRE(*string == "a\nbc");
}
