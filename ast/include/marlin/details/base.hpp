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

  template <typename node_type>
  struct impl;

  [[nodiscard]] inline size_t type() const { return _typeid; }

  template <typename node_type>
  [[nodiscard]] inline bool is() const {
    return type() == get_typeid<node_type>();
  }
  template <typename node_type>
  [[nodiscard]] inline node_type &as() {
    return *static_cast<node_type *>(this);
  }
  template <typename node_type>
  [[nodiscard]] inline const node_type &as() const {
    return *static_cast<const node_type *>(this);
  }

  template <typename return_type, typename callable_type>
  inline return_type apply(callable_type callable);
  template <typename return_type, typename callable_type>
  inline return_type apply(callable_type callable) const;

  template <typename super_type>
  [[nodiscard]] inline bool inherits() const {
    return apply<bool>([](const auto &n) {
      return std::is_base_of_v<super_type, std::decay_t<decltype(n)>>;
    });
  }

  [[nodiscard]] inline bool is_valid_child(const base &n, size_t index) const {
    return apply<bool>([&n, &index](const auto &self) {
      return self.is_valid_child(n, index);
    });
  }

  [[nodiscard]] inline source_range source_code_range() const noexcept {
    return _source_range;
  }
  [[nodiscard]] inline bool contains(source_loc loc) const noexcept {
    return _source_range.contains(loc);
  }

  [[nodiscard]] base &locate(source_loc loc);
  [[nodiscard]] const base &locate(source_loc loc) const;

  [[nodiscard]] base &locate_js(source_loc loc);
  [[nodiscard]] const base &locate_js(source_loc loc) const;

  [[nodiscard]] inline bool has_parent() const noexcept {
    return _parent != nullptr;
  }
  [[nodiscard]] inline base &parent() { return *_parent; }

  [[nodiscard]] inline utils::access_vector_view<utils::move_vector<node>>
  children() {
    return {_children};
  }
  [[nodiscard]] inline utils::const_vector_view<utils::move_vector<node>>
  children() const {
    return {_children};
  }

  inline node replace_child(node replacement, size_t target_index) {
    auto prev{std::move(_children[target_index])};
    prev->_parent = nullptr;
    replacement->_parent = this;
    _children[target_index] = std::move(replacement);
    return prev;
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

 protected:
  inline void push_back_child(node child) {
    child->_parent = this;
    _children.emplace_back(std::move(child));
  }

  utils::move_vector<node> _children;

 private:
  size_t _typeid;

  base *_parent{nullptr};

  source_range _source_range;
  source_range _js_range;

  explicit inline base(size_t tid) : _typeid{tid} {}

  template <typename node_type>
  [[nodiscard]] static inline constexpr size_t get_typeid() noexcept;
};

template <typename node_type>
struct base::impl : base {
 protected:
  explicit inline impl() : base{get_typeid<node_type>()} {}
};

}  // namespace ast

}  // namespace marlin

#endif  // marlin_ast_base_hpp