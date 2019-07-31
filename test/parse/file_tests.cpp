#include <catch2/catch.hpp>

#include <marlin/parse.hpp>

TEST_CASE("parse::Report non-existent files", "[parse]") {
  REQUIRE_THROWS_AS(marlin::parse::process_file("non_existent_file.mar"),
                    marlin::parse::filesystem_error);
}

TEST_CASE("parse::Parse file", "[parse]") {
  const auto [code, errors]{marlin::parse::process_file("existent_file.mar")};
  REQUIRE(errors.size() == 0);

  REQUIRE(code.is<marlin::ast::program>());
  REQUIRE(code->children_count() == 1);
  CHECK(code->child(0).is<marlin::ast::expression_statement>());
  REQUIRE(code->child(0)->children_count() == 1);
  CHECK(code->child(0)->child(0).inherits<marlin::ast::expression>());
}