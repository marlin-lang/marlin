#ifndef marlin_ast_node_hpp
#define marlin_ast_node_hpp

#include <memory>

namespace marlin {

namespace ast {

struct base;

struct node {
  // Allows default construction of node
  inline node();

  template <typename node_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  inline node(node_type &&n)
      : _node{new node_type{std::forward<node_type>(n)}},
        _typeid{get_typeid<std::decay_t<node_type>>()} {}

  inline ~node() {
    if (_node != nullptr) {
      apply<void>([](auto &n) { delete &n; });
    }
  }

  inline node(const node &n) = delete;
  inline node(node &&n) noexcept : _node{n._node}, _typeid{n._typeid} {
    n._node = nullptr;
  }
  inline node &operator=(const node &n) = delete;
  inline node &operator=(node &&n) noexcept {
    std::swap(_node, n._node);
    std::swap(_typeid, n._typeid);
    return *this;
  }

  inline base *operator->() { return _node; }
  inline const base *operator->() const { return _node; }

  [[nodiscard]] inline base &get() { return *_node; }
  [[nodiscard]] inline const base &get() const { return *_node; }
  [[nodiscard]] inline size_t type() const { return _typeid; }

  template <typename node_type>
  [[nodiscard]] inline bool is() const {
    return type() == get_typeid<node_type>();
  }
  template <typename node_type>
  [[nodiscard]] inline node_type &as() {
    return static_cast<node_type &>(get());
  }
  template <typename node_type>
  [[nodiscard]] inline const node_type &as() const {
    return static_cast<const node_type &>(get());
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

  [[nodiscard]] inline bool is_valid_child(const node &n, size_t index) const {
    return apply<bool>([&n, &index](const auto &self) {
      return self.is_valid_child(n, index);
    });
  }

 private:
  base *_node;
  size_t _typeid;

  template <typename node_type>
  [[nodiscard]] static inline constexpr size_t get_typeid() noexcept;
};

}  // namespace ast

using code = ast::node;

}  // namespace marlin

#endif  // marlin_ast_node_hpp