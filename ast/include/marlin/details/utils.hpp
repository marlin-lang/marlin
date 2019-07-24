#ifndef marlin_ast_utils_hpp
#define marlin_ast_utils_hpp

#include <jsast/jsast.hpp>

namespace marlin {

using jsast::source_loc;
using jsast::source_range;

namespace utils {

using jsast::utils::move_vector;

}  // namespace utils

}  // namespace marlin

#endif  // marlin_ast_utils_hpp