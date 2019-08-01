#ifndef marlin_ast_node_hpp
#define marlin_ast_node_hpp

#include <memory>

namespace marlin {

namespace ast {

struct base;

struct node {
  template <typename node_type, typename... arg_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  [[nodiscard]] static inline node make(arg_type &&... args) {
    return {new node_type{std::forward<arg_type>(args)...}};
  }

  [[nodiscard]] static node make_empty();

  ~node();

  inline node(const node &n) = delete;
  inline node(node &&n) noexcept : _node{n._node} { n._node = nullptr; }
  inline node &operator=(const node &n) = delete;
  inline node &operator=(node &&n) noexcept {
    std::swap(_node, n._node);
    return *this;
  }

  inline base *operator->() { return _node; }
  inline const base *operator->() const { return _node; }

  [[nodiscard]] inline base &get() { return *_node; }
  [[nodiscard]] inline const base &get() const { return *_node; }

 private:
  base *_node;

  template <typename node_type,
            typename = std::enable_if_t<std::is_base_of_v<base, node_type>>>
  inline node(node_type *n) : _node{n} {}
};

}  // namespace ast

using code = ast::node;

}  // namespace marlin

#endif  // marlin_ast_node_hpp