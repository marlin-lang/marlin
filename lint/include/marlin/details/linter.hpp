#ifndef marlin_lint_linter_hpp
#define marlin_lint_linter_hpp

#include <marlin/ast.hpp>

namespace marlin::lint {

struct linter {
  inline linter(code c) : _code{std::move(c)} {}

  code lint() && { return std::move(_code); }

 private:
  code _code;

  inline void lint_tree(code& node) {
    // TODO: Lint sub-nodes differently for each type
    for (size_t i{0}; i < node->children_count(); i++) {
      lint_tree(node->child(i));
    }
  }
};

}  // namespace marlin::lint

#endif  // marlin_lint_linter_hpp