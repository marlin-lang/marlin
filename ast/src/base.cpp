#include "base.hpp"

// To provide full definition for struct base
#include "ast.hpp"
#include "base.inc.hpp"

namespace marlin::ast {

base &base::locate(source_loc loc) {
  for (auto &child : children()) {
    if (child->contains(loc)) {
      return child->locate(loc);
    }
  }
  return *this;
}

const base &base::locate(source_loc loc) const {
  for (const auto &child : children()) {
    if (child->contains(loc)) {
      return child->locate(loc);
    }
  }
  return *this;
}

base &base::locate_js(source_loc loc) {
  for (auto &child : children()) {
    if (child->_js_range.contains(loc)) {
      return child->locate_js(loc);
    }
  }
  return *this;
}

const base &base::locate_js(source_loc loc) const {
  for (const auto &child : children()) {
    if (child->_js_range.contains(loc)) {
      return child->locate_js(loc);
    }
  }
  return *this;
}

}  // namespace marlin::ast
