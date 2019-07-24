#ifndef marlin_ast_node_hpp
#define marlin_ast_node_hpp

#include <memory>

namespace marlin {

namespace ast {

struct base;

struct node {
  template <typename node_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  inline node(node_type&& n)
      : _node{new node_type{std::forward<node_type>(n)}},
        _typeid{get_typeid<std::decay_t<node_type>>()} {}

  inline ~node() {
    if (_node != nullptr) {
      apply<void>([](auto& n) { delete &n; });
    }
  }

  inline node(const node& n) = delete;
  inline node(node&& n) noexcept : _node{n._node}, _typeid{n._typeid} {
    n._node = nullptr;
  }
  inline node& operator=(const node& n) = delete;
  inline node& operator=(node&& n) noexcept {
    std::swap(_node, n._node);
    std::swap(_typeid, n._typeid);
    return *this;
  }

  inline base& get() { return *_node; }
  inline const base& get() const { return *_node; }
  inline size_t type() const { return _typeid; }

  template <typename node_type>
  inline bool is() const {
    return type() == get_typeid<node_type>();
  }
  template <typename node_type>
  inline node_type& as() {
    return static_cast<node_type&>(get());
  }
  template <typename node_type>
  inline const node_type& as() const {
    return static_cast<const node_type&>(get());
  }

  template <typename return_type, typename callable_type>
  inline return_type apply(callable_type callable);
  template <typename return_type, typename callable_type>
  inline return_type apply(callable_type callable) const;

 private:
  base* _node;
  size_t _typeid;

  template <typename node_type>
  static inline constexpr size_t get_typeid() noexcept;
};

}  // namespace ast

using code = ast::node;

}  // namespace marlin

#endif  // marlin_ast_node_hpp