#include <catch2/catch.hpp>

#include <marlin/parse.hpp>

const marlin::ast::base* find_expression(const marlin::code& c) {
  if (c->children().size() == 1 && c->children()[0]->children().size() == 1 &&
      c->children()[0]->children()[0]->inherits<marlin::ast::expression>()) {
    return &c->children()[0]->children()[0].get();
  } else {
    return nullptr;
  }
}

TEST_CASE("parse::Parse simple numeric expression", "[parse]") {
  const auto [code, errors]{marlin::parse::process("5 / 6;")};
  REQUIRE(errors.size() == 0);

  const auto* ptr{find_expression(code)};
  REQUIRE(ptr != nullptr);
  const auto& expr{*ptr};
  REQUIRE(expr.is<marlin::ast::binary_expression>());
  CHECK(expr.as<marlin::ast::binary_expression>().op ==
        marlin::ast::binary_op::divide);
  REQUIRE(expr.children().size() == 2);
  REQUIRE(expr.children()[0]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[0]->as<marlin::ast::number_literal>().number == "5");
  REQUIRE(expr.children()[0]->children().size() == 0);
  REQUIRE(expr.children()[1]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[1]->as<marlin::ast::number_literal>().number == "6");
  REQUIRE(expr.children()[0]->children().size() == 0);
}

TEST_CASE("parse::Parse numeric expression with precedence", "[parse]") {
  const auto [code, errors]{marlin::parse::process("5 + 6 * 7 + 8;")};
  REQUIRE(errors.size() == 0);

  const auto* ptr{find_expression(code)};
  REQUIRE(ptr != nullptr);
  const auto& expr{*ptr};
  REQUIRE(expr.is<marlin::ast::binary_expression>());
  CHECK(expr.as<marlin::ast::binary_expression>().op ==
        marlin::ast::binary_op::add);
  REQUIRE(expr.children().size() == 2);
  REQUIRE(expr.children()[0]->is<marlin::ast::binary_expression>());
  CHECK(expr.children()[0]->as<marlin::ast::binary_expression>().op ==
        marlin::ast::binary_op::add);
  REQUIRE(expr.children()[0]->children().size() == 2);
  REQUIRE(expr.children()[0]->children()[0]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[0]
            ->children()[0]
            ->as<marlin::ast::number_literal>()
            .number == "5");
  REQUIRE(expr.children()[0]->children()[0]->children().size() == 0);
  REQUIRE(
      expr.children()[0]->children()[1]->is<marlin::ast::binary_expression>());
  CHECK(expr.children()[0]
            ->children()[1]
            ->as<marlin::ast::binary_expression>()
            .op == marlin::ast::binary_op::multiply);
  REQUIRE(expr.children()[0]->children()[1]->children().size() == 2);
  REQUIRE(expr.children()[0]
              ->children()[1]
              ->children()[0]
              ->is<marlin::ast::number_literal>());
  CHECK(expr.children()[0]
            ->children()[1]
            ->children()[0]
            ->as<marlin::ast::number_literal>()
            .number == "6");
  REQUIRE(expr.children()[0]->children()[1]->children()[0]->children().size() ==
          0);
  REQUIRE(expr.children()[0]
              ->children()[1]
              ->children()[1]
              ->is<marlin::ast::number_literal>());
  CHECK(expr.children()[0]
            ->children()[1]
            ->children()[1]
            ->as<marlin::ast::number_literal>()
            .number == "7");
  REQUIRE(expr.children()[0]->children()[1]->children()[1]->children().size() ==
          0);
  REQUIRE(expr.children()[1]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[1]->as<marlin::ast::number_literal>().number == "8");
  REQUIRE(expr.children()[1]->children().size() == 0);
}

TEST_CASE("parse::Parse numeric expression with parentheses", "[parse]") {
  const auto [code, errors]{marlin::parse::process("(5 + 6) * 7;")};
  REQUIRE(errors.size() == 0);

  const auto* ptr{find_expression(code)};
  REQUIRE(ptr != nullptr);
  const auto& expr{*ptr};
  REQUIRE(expr.is<marlin::ast::binary_expression>());
  CHECK(expr.as<marlin::ast::binary_expression>().op ==
        marlin::ast::binary_op::multiply);
  REQUIRE(expr.children().size() == 2);
  REQUIRE(expr.children()[0]->is<marlin::ast::binary_expression>());
  CHECK(expr.children()[0]->as<marlin::ast::binary_expression>().op ==
        marlin::ast::binary_op::add);
  REQUIRE(expr.children()[0]->children().size() == 2);
  REQUIRE(expr.children()[0]->children()[0]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[0]
            ->children()[0]
            ->as<marlin::ast::number_literal>()
            .number == "5");
  REQUIRE(expr.children()[0]->children()[0]->children().size() == 0);
  REQUIRE(expr.children()[0]->children()[1]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[0]
            ->children()[1]
            ->as<marlin::ast::number_literal>()
            .number == "6");
  REQUIRE(expr.children()[0]->children()[1]->children().size() == 0);
  REQUIRE(expr.children()[1]->is<marlin::ast::number_literal>());
  CHECK(expr.children()[1]->as<marlin::ast::number_literal>().number == "7");
  REQUIRE(expr.children()[1]->children().size() == 0);
}
