#include "node.hpp"

#include "base.hpp"

// To give a complete definition for ast::base
#include "ast.hpp"
#include "base.inc.hpp"

namespace marlin::ast {

node node::make_empty() {
  return make<program>(marlin::utils::move_vector<node>{});
}

node::~node() {
  if (_node != nullptr) {
    _node->apply<void>([](auto &n) { delete &n; });
  }
}

}  // namespace marlin::ast
