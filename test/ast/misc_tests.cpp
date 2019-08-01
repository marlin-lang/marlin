#include <catch2/catch.hpp>

#include <cstring>

#include <marlin/ast.hpp>

TEST_CASE("ast::Evaluate unary operator lengths", "[ast]") {
  CHECK(marlin::ast::length_of(marlin::ast::unary_op::positive) == strlen("+"));
  CHECK(marlin::ast::length_of(marlin::ast::unary_op::negative) == strlen("-"));
}

TEST_CASE("ast::Evaluate binary operators lengths", "[ast]") {
  CHECK(marlin::ast::length_of(marlin::ast::binary_op::add) == strlen("+"));
  CHECK(marlin::ast::length_of(marlin::ast::binary_op::subtract) ==
        strlen("-"));
  CHECK(marlin::ast::length_of(marlin::ast::binary_op::multiply) ==
        strlen("*"));
  CHECK(marlin::ast::length_of(marlin::ast::binary_op::divide) == strlen("/"));
}

TEST_CASE("ast::Test node inheritance", "[ast]") {
  auto statement = marlin::code::make<marlin::ast::expression_statement>(
      marlin::code::make<marlin::ast::string_literal>("x"));
  auto expression = marlin::code::make<marlin::ast::string_literal>("x");

  CHECK(statement->inherits<marlin::ast::statement>());
  CHECK_FALSE(statement->inherits<marlin::ast::expression>());
  CHECK_FALSE(expression->inherits<marlin::ast::statement>());
  CHECK(expression->inherits<marlin::ast::expression>());
}