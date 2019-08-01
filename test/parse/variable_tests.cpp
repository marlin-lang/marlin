#include <catch2/catch.hpp>

#include <marlin/parse.hpp>

TEST_CASE("parse::Parse variable declaration", "[parse]") {
  const auto [code, errors] = marlin::parse::process("let a = 1;");
  REQUIRE(errors.size() == 0);
}

TEST_CASE("parse::Parse multiple variable declaration", "[parse]") {
  const auto [code, errors] = marlin::parse::process("let a = 1, b = \"2\";");
  REQUIRE(errors.size() == 0);
}