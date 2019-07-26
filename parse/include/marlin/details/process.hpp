#ifndef marlin_parse_process_hpp
#define marlin_parse_process_hpp

#include <fstream>
#include <string>

#include "interpreter.hpp"
#include "scanner.hpp"

namespace marlin::parse {

inline code process(const std::string& source) {
  scanner sc{source};
  interpreter i{std::move(sc)};
  return i.parse();
}

inline code process_file(const std::string filename) {
  // TODO: error handling
  std::ifstream input{filename,
                      std::ios::in | std::ios::binary | std::ios::ate};
  const auto size{input.tellg()};
  input.seekg(0, std::ios::beg);
  std::string source(size, '\0');
  input.read(source.data(), size);
  return process(source);
}

}  // namespace marlin::parse

#endif  //  marlin_parse_process_hpp