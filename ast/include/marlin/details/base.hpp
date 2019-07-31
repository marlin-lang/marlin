#ifndef marlin_ast_base_hpp
#define marlin_ast_base_hpp

#include <type_traits>

#include "node.hpp"
#include "utils.hpp"

namespace marlin {

namespace parse {

struct interpreter;

}  // namespace parse

namespace lint {

struct linter;

}  // namespace lint

namespace exec {

struct generator;

}  // namespace exec

namespace ast {

struct base {
  friend parse::interpreter;
  friend lint::linter;
  friend exec::generator;

  friend node;

  explicit inline base(utils::move_vector<node> children)
      : _children{std::move(children)} {}
  explicit inline base(size_t children_count) {
    _children.reserve(children_count);
  }

  [[nodiscard]] inline source_range source_code_range() const noexcept {
    return _source_range;
  }
  [[nodiscard]] inline bool contains(source_loc loc) const noexcept {
    return _source_range.contains(loc);
  }

  [[nodiscard]] inline bool has_parent() const noexcept {
    return _parent != nullptr;
  }
  [[nodiscard]] inline base &parent() { return *_parent; }

  [[nodiscard]] inline size_t children_count() const noexcept {
    return _children.size();
  }
  [[nodiscard]] inline node &child(size_t index) { return _children[index]; }
  [[nodiscard]] inline const node &child(size_t index) const {
    return _children[index];
  }
  inline void push_child(node &&child) {
    _children.emplace_back(std::move(child));
  }

  inline node replace_child(node replacement, size_t target_index) {
    auto prev{std::move(_children[target_index])};
    prev->_parent = nullptr;
    replacement->_parent = this;
    _children[target_index] = std::move(replacement);
    return prev;
  }

  inline node replace_child(node replacement, const node &target) {
    for (size_t i{0}; i < _children.size(); i++) {
      if (&_children[i] == &target) {
        return replace_child(std::move(replacement), i);
      }
    }
    /* should not occur! */
    return replacement;
  }

  inline node replace_child(node replacement, const base &target) {
    for (size_t i{0}; i < _children.size(); i++) {
      if (&_children[i].get() == &target) {
        return replace_child(std::move(replacement), i);
      }
    }
    /* should not occur! */
    return replacement;
  }

  inline node replace(node replacement) {
    if (has_parent()) {
      return parent().replace_child(std::move(replacement), *this);
    } else {
      /* should not occur */
      return replacement;
    }
  }

 private:
  utils::move_vector<node> _children;
  base *_parent{nullptr};

  source_range _source_range;
  source_range _js_range;
};

}  // namespace ast

}  // namespace marlin

#endif  // marlin_ast_base_hpp