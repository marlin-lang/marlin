#ifndef marlin_parse_process_hpp
#define marlin_parse_process_hpp

#include <fstream>
#include <string>

#include "errors.hpp"
#include "interpreter.hpp"
#include "scanner.hpp"

namespace marlin::parse {

inline auto process(const std::string& source) {
  scanner sc{source};
  interpreter i{std::move(sc)};
  return i.parse();
}

inline auto process_file(const std::string filename) {
  std::ifstream input{filename,
                      std::ios::in | std::ios::binary | std::ios::ate};
  if (!input.is_open()) {
    throw filesystem_error{"Cannot read file \"" + filename + "\"!"};
  }

  const auto size{input.tellg()};
  if (size < 0) {
    throw filesystem_error{"Cannot read file \"" + filename + "\"!"};
  }

  input.seekg(0, std::ios::beg);
  std::string source(size, '\0');
  input.read(source.data(), size);
  if (input.fail()) {
    throw filesystem_error{"Cannot read file \"" + filename + "\"!"};
  }

  return process(source);
}  // namespace marlin::parse

}  // namespace marlin::parse

#endif  //  marlin_parse_process_hpp