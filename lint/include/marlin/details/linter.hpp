#ifndef marlin_lint_linter_hpp
#define marlin_lint_linter_hpp

#include <iostream>

#include <marlin/ast.hpp>

namespace marlin::lint {

struct linter {
  inline linter(ast::base& c) noexcept : _code{c} {}

  void lint() { lint_tree(_code); }

 private:
  ast::base& _code;

  inline void lint_tree(ast::base& node) {
    const auto children{node.children()};
    for (size_t i{0}; i < children.size(); i++) {
      if (!node.is_valid_child(children[i].get(), i)) {
        // Error reporting
        const auto loc{children[i]->source_code_range()};
        std::cout << "Invalid node found at " << loc.begin.line << ":"
                  << loc.begin.column << "\n";
      }
    }

    // TODO: Lint sub-nodes differently for each type
    for (auto& child : node.children()) {
      lint_tree(child.get());
    }
  }
};

}  // namespace marlin::lint

#endif  // marlin_lint_linter_hpp