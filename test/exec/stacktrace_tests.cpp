#include <catch2/catch.hpp>

#include <marlin/exec.hpp>

namespace marlin::exec {
// Private API to test
std::vector<marlin::source_loc> parse_stacktrace(const std::string& stacktrace,
                                                 const std::string& source_url);
}  // namespace marlin::exec

TEST_CASE("exec::Multiple stacktraces", "[exec]") {
  const std::string s =
      "[native code]\n"
      "global function@<anonymous>:10:7\n"
      "g@<anonymous>:160:80";

  const auto result = marlin::exec::parse_stacktrace(s, "<anonymous>");
  REQUIRE(result.size() == 2);
  CHECK(result[0].line == 10);
  CHECK(result[0].column == 7);
  CHECK(result[1].line == 160);
  CHECK(result[1].column == 80);
}

TEST_CASE("exec::Filter stacktraces", "[exec]") {
  const std::string s =
      "[native code]\n"
      "f@<anonymous>:10:7\n"
      "g@../folder/test.mar:160:80\n";

  const auto result = marlin::exec::parse_stacktrace(s, "../folder/test.mar");
  REQUIRE(result.size() == 1);
  CHECK(result[0].line == 160);
  CHECK(result[0].column == 80);
}

TEST_CASE("exec::Stacktraces with special filenames", "[exec]") {
  const std::string s =
      "[native code]\n"
      "f@<anonymous>:10:7\n"
      "g@../folder/t@st.mar:160:80\n"
      "global function@../folder/test.mar:5:17\n"
      "h@../folder/t@st.mar:20:6\n";

  const auto result = marlin::exec::parse_stacktrace(s, "../folder/t@st.mar");
  REQUIRE(result.size() == 2);
  CHECK(result[0].line == 160);
  CHECK(result[0].column == 80);
  CHECK(result[1].line == 20);
  CHECK(result[1].column == 6);
}
