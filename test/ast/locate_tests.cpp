#include <catch2/catch.hpp>

#include <marlin/parse.hpp>

TEST_CASE("ast::Locate nodes by source_loc", "[ast]") {
  const auto [code, errors]{
      marlin::parse::process("print(5 + 6 * .7 + abc);\nprint(\"\\n\");")};
  REQUIRE(errors.size() == 0);

  REQUIRE(&code->locate({1, 15}) == &code->children()[0]
                                         ->children()[0]
                                         ->children()[1]
                                         ->children()[0]
                                         ->children()[1]
                                         ->children()[1]
                                         .get());
  REQUIRE(&code->locate({1, 23}) == &code->children()[0]->children()[0].get());
  REQUIRE(&code->locate({1, 24}) == &code->children()[0].get());
  REQUIRE(&code->locate({1, 25}) == &code.get());
  REQUIRE(&code->locate({2, 10}) ==
          &code->children()[1]->children()[0]->children()[1].get());
}