#ifndef marlin_parse_process_hpp
#define marlin_parse_process_hpp

#include <string>

#include "interpreter.hpp"
#include "scanner.hpp"

namespace marlin::parse {

inline code process(const std::string& source) {
  scanner sc{source};
  interpreter i{std::move(sc)};
  return i.parse();
}

}  // namespace marlin::parse

#endif  //  marlin_parse_process_hpp