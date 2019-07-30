#include <catch2/catch.hpp>

#include <string>

#include <marlin/parse.hpp>

const marlin::code* find_expression(const marlin::code& c) {
  if (c->children_count() == 1 && c->child(0)->children_count() == 1 &&
      c->child(0)->child(0).inherits<marlin::ast::expression>()) {
    return &c->child(0)->child(0);
  } else {
    return nullptr;
  }
}

TEST_CASE("parse::Parse simple numeric expression", "[parse]") {
  const auto [code, errors] = marlin::parse::process("5 / 6;");
  REQUIRE(errors.size() == 0);

  const auto* ptr = find_expression(code);
  REQUIRE(ptr != nullptr);
  const auto& expr = *ptr;
  REQUIRE(expr.is<marlin::ast::binary_expression>());
  REQUIRE(expr.as<marlin::ast::binary_expression>().op ==
          marlin::ast::binary_op::divide);
  REQUIRE(expr->children_count() == 2);
  REQUIRE(expr->child(0).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(0).as<marlin::ast::number_literal>().number == "5");
  REQUIRE(expr->child(0)->children_count() == 0);
  REQUIRE(expr->child(1).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(1).as<marlin::ast::number_literal>().number == "6");
  REQUIRE(expr->child(0)->children_count() == 0);
}

TEST_CASE("parse::Parse numeric expression with precedence", "[parse]") {
  const auto [code, errors] = marlin::parse::process("5 + 6 * 7 + 8;");
  REQUIRE(errors.size() == 0);

  const auto* ptr = find_expression(code);
  REQUIRE(ptr != nullptr);
  const auto& expr = *ptr;
  REQUIRE(expr.is<marlin::ast::binary_expression>());
  REQUIRE(expr.as<marlin::ast::binary_expression>().op ==
          marlin::ast::binary_op::add);
  REQUIRE(expr->children_count() == 2);
  REQUIRE(expr->child(0).is<marlin::ast::binary_expression>());
  REQUIRE(expr->child(0).as<marlin::ast::binary_expression>().op ==
          marlin::ast::binary_op::add);
  REQUIRE(expr->child(0)->children_count() == 2);
  REQUIRE(expr->child(0)->child(0).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(0)->child(0).as<marlin::ast::number_literal>().number ==
          "5");
  REQUIRE(expr->child(0)->child(0)->children_count() == 0);
  REQUIRE(expr->child(0)->child(1).is<marlin::ast::binary_expression>());
  REQUIRE(expr->child(0)->child(1).as<marlin::ast::binary_expression>().op ==
          marlin::ast::binary_op::multiply);
  REQUIRE(expr->child(0)->child(1)->children_count() == 2);
  REQUIRE(expr->child(0)->child(1)->child(0).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(0)
              ->child(1)
              ->child(0)
              .as<marlin::ast::number_literal>()
              .number == "6");
  REQUIRE(expr->child(0)->child(1)->child(0)->children_count() == 0);
  REQUIRE(expr->child(0)->child(1)->child(1).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(0)
              ->child(1)
              ->child(1)
              .as<marlin::ast::number_literal>()
              .number == "7");
  REQUIRE(expr->child(0)->child(1)->child(1)->children_count() == 0);
  REQUIRE(expr->child(1).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(1).as<marlin::ast::number_literal>().number == "8");
  REQUIRE(expr->child(1)->children_count() == 0);
}

TEST_CASE("parse::Parse numeric expression with parentheses", "[parse]") {
  const auto [code, errors] = marlin::parse::process("(5 + 6) * 7;");
  REQUIRE(errors.size() == 0);

  const auto* ptr = find_expression(code);
  REQUIRE(ptr != nullptr);
  const auto& expr = *ptr;
  REQUIRE(expr.is<marlin::ast::binary_expression>());
  REQUIRE(expr.as<marlin::ast::binary_expression>().op ==
          marlin::ast::binary_op::multiply);
  REQUIRE(expr->children_count() == 2);
  REQUIRE(expr->child(0).is<marlin::ast::binary_expression>());
  REQUIRE(expr->child(0).as<marlin::ast::binary_expression>().op ==
          marlin::ast::binary_op::add);
  REQUIRE(expr->child(0)->children_count() == 2);
  REQUIRE(expr->child(0)->child(0).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(0)->child(0).as<marlin::ast::number_literal>().number ==
          "5");
  REQUIRE(expr->child(0)->child(0)->children_count() == 0);
  REQUIRE(expr->child(0)->child(1).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(0)->child(1).as<marlin::ast::number_literal>().number ==
          "6");
  REQUIRE(expr->child(0)->child(1)->children_count() == 0);
  REQUIRE(expr->child(1).is<marlin::ast::number_literal>());
  REQUIRE(expr->child(1).as<marlin::ast::number_literal>().number == "7");
  REQUIRE(expr->child(1)->children_count() == 0);
}
