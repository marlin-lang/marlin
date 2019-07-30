#ifndef marlin_lint_linter_hpp
#define marlin_lint_linter_hpp

#include <marlin/ast.hpp>

namespace marlin::lint {

struct linter {
  inline linter(code& c) noexcept : _code{c} {}

  void lint() { lint_tree(_code); }

 private:
  code& _code;

  inline void lint_tree(code& node) {
    for (size_t i{0}; i < node->children_count(); i++) {
      if (!node.is_valid_child(node->child(i), i)) {
        // Error reporting
        const auto loc{node->child(i)->source_code_range()};
        std::cout << "Invalid node found at " << loc.begin.line << ":"
                  << loc.begin.column << "\n";
      }
    }

    // TODO: Lint sub-nodes differently for each type
    for (size_t i{0}; i < node->children_count(); i++) {
      lint_tree(node->child(i));
    }
  }
};

}  // namespace marlin::lint

#endif  // marlin_lint_linter_hpp