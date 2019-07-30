#include "node.hpp"

#include "base.hpp"

// To provide full definition for struct node
#include "ast.hpp"
#include "node.inc.hpp"

namespace marlin::ast {

node &node::locate(source_loc loc) {
  for (size_t i{0}; i < get().children_count(); i++) {
    auto &child = get().child(i);
    if (child->contains(loc)) {
      return child.locate(loc);
    }
  }
  return *this;
}

const node &node::locate(source_loc loc) const {
  for (size_t i{0}; i < get().children_count(); i++) {
    const auto &child = get().child(i);
    if (child->contains(loc)) {
      return child.locate(loc);
    }
  }
  return *this;
}

}  // namespace marlin::ast
