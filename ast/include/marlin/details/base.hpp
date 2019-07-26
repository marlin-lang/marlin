#ifndef marlin_ast_base_hpp
#define marlin_ast_base_hpp

#include <type_traits>

#include "node.hpp"
#include "utils.hpp"

namespace marlin {

namespace exec {

struct generator;

}  // namespace exec

namespace ast {

struct base {
  friend exec::generator;

  explicit inline base(size_t children_count) {
    _children.reserve(children_count);
  }

  inline source_range source_code_range() const noexcept {
    return _source_range;
  }
  inline source_range js_range() const noexcept { return _js_range; }

  inline bool has_parent() const noexcept { return _parent != nullptr; }
  inline base& parent() { return *_parent; }

  inline node replace_child(node replacement, size_t target_index) {
    auto prev{std::move(_children[target_index])};
    prev.get()._parent = nullptr;
    replacement.get()._parent = this;
    _children[target_index] = std::move(replacement);
    return std::move(prev);
  }

  inline node replace_child(node replacement, const node& target) {
    for (size_t i{0}; i < _children.size(); i++) {
      if (&_children[i] == &target) {
        return replace_child(std::move(replacement), i);
      }
    }
    /* should not occur! */
    return std::move(replacement);
  }

  inline node replace_child(node&& replacement, const base& target) {
    for (size_t i{0}; i < _children.size(); i++) {
      if (&_children[i].get() == &target) {
        return replace_child(std::move(replacement), i);
      }
    }
    /* should not occur! */
    return std::move(replacement);
  }

  inline node replace(node replacement) {
    if (has_parent()) {
      return parent().replace_child(std::move(replacement), *this);
    } else {
      /* should not occur */
      return std::move(replacement);
    }
  }

 protected:
  utils::move_vector<node> _children;

 private:
  base* _parent{nullptr};

  source_range _source_range;
  source_range _js_range;
};

}  // namespace ast

}  // namespace marlin

#endif  // marlin_ast_base_hpp