#ifndef marlin_ast_base_hpp
#define marlin_ast_base_hpp

#include <optional>
#include <type_traits>

#include "node.hpp"
#include "subnodes.hpp"
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
  template <typename... arg_type>
  inline void init(arg_type &&... args) {
    _children.reserve(count_subnodes(args...));
    do_init(std::forward<arg_type>(args)...);
  }

  inline node &get_subnode(subnode::ref &r) { return _children[r.index]; }
  inline const node &get_subnode(const subnode::ref &r) const {
    return _children[r.index];
  }

  utils::move_vector<node> _children;

 private:
  size_t _typeid;

  base *_parent{nullptr};

  source_range _source_range;
  source_range _js_range;

  explicit inline base(size_t tid) : _typeid{tid} {}

  [[nodiscard]] inline size_t count_subnodes() { return 0; }
  template <typename... arg_type>
  [[nodiscard]] inline size_t count_subnodes(subnode::ref &, const node &,
                                             arg_type &&... args) {
    return 1 + count_subnodes(std::forward<arg_type>(args)...);
  }
  template <typename... arg_type>
  [[nodiscard]] inline size_t count_subnodes(subnode::optional &,
                                             const std::optional<node> &value,
                                             arg_type &&... args) {
    return (value.has_value() ? 1 : 0) +
           count_subnodes(std::forward<arg_type>(args)...);
  }
  template <typename... arg_type>
  [[nodiscard]] inline size_t count_subnodes(
      subnode::vector &, const utils::move_vector<node> &value,
      arg_type &&... args) {
    return value.size() + count_subnodes(std::forward<arg_type>(args)...);
  }

  inline void do_init() const noexcept {}
  template <typename... arg_type>
  inline void do_init(subnode::ref &var, node value, arg_type &&... args) {
    var.index = _children.size();
    _children.emplace_back(std::move(value));
    do_init(std::forward<arg_type>(args)...);
  }
  template <typename... arg_type>
  inline void do_init(subnode::optional &var, std::optional<node> value,
                      arg_type &&... args) {
    var.index = _children.size();
    if (value.has_value()) {
      var.has_value = true;
      _children.emplace_back(std::move(value).value());
    } else {
      var.has_value = false;
    }
    do_init(std::forward<arg_type>(args)...);
  }
  template <typename... arg_type>
  inline void do_init(subnode::vector &var, utils::move_vector<node> value,
                      arg_type &&... args) {
    var.index = _children.size();
    var.size = value.size();
    std::move(value.begin(), value.end(), std::back_inserter(_children));
    do_init(std::forward<arg_type>(args)...);
  }

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