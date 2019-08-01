#include <catch2/catch.hpp>

#include <marlin/format.hpp>
#include <marlin/lint.hpp>
#include <marlin/parse.hpp>

TEST_CASE("format::Highlight numbers and operators", "[format]") {
  const std::string script{"print(1 + 2 - 3);"};

  auto [code, parse_errors]{marlin::parse::process(script)};
  REQUIRE(parse_errors.size() == 0);

  marlin::lint::linter l{code.get()};
  l.lint();

  marlin::format::highlight hl{code.get()};
  auto highlights = hl.generate();
  REQUIRE(highlights.size() == 5);
  CHECK(highlights[0].type == marlin::format::highlight::token_type::number);
  CHECK(highlights[0].range.begin.line == 1);
  CHECK(highlights[0].range.begin.column == 7);
  CHECK(highlights[0].range.end.line == 1);
  CHECK(highlights[0].range.end.column == 8);
  CHECK(highlights[1].type == marlin::format::highlight::token_type::op);
  CHECK(highlights[1].range.begin.line == 1);
  CHECK(highlights[1].range.begin.column == 9);
  CHECK(highlights[1].range.end.line == 1);
  CHECK(highlights[1].range.end.column == 10);
  CHECK(highlights[2].type == marlin::format::highlight::token_type::number);
  CHECK(highlights[2].range.begin.line == 1);
  CHECK(highlights[2].range.begin.column == 11);
  CHECK(highlights[2].range.end.line == 1);
  CHECK(highlights[2].range.end.column == 12);
  CHECK(highlights[3].type == marlin::format::highlight::token_type::op);
  CHECK(highlights[3].range.begin.line == 1);
  CHECK(highlights[3].range.begin.column == 13);
  CHECK(highlights[3].range.end.line == 1);
  CHECK(highlights[3].range.end.column == 14);
  CHECK(highlights[4].type == marlin::format::highlight::token_type::number);
  CHECK(highlights[4].range.begin.line == 1);
  CHECK(highlights[4].range.begin.column == 15);
  CHECK(highlights[4].range.end.line == 1);
  CHECK(highlights[4].range.end.column == 16);
}
