#include <catch2/catch.hpp>

#include <marlin/parse.hpp>

TEST_CASE("ast::Locate nodes by source_loc", "[ast]") {
  const auto [code, errors]{
      marlin::parse::process("print(5 + 6 * .7 + abc);\nprint(\"\\n\");")};
  REQUIRE(errors.size() == 0);

  REQUIRE(&code.locate({1, 15}) ==
          &code->child(0)->child(0)->child(1)->child(0)->child(1)->child(1));
  REQUIRE(&code.locate({1, 23}) == &code->child(0)->child(0));
  REQUIRE(&code.locate({1, 24}) == &code->child(0));
  REQUIRE(&code.locate({1, 25}) == &code);
  REQUIRE(&code.locate({2, 10}) == &code->child(1)->child(0)->child(1));
}