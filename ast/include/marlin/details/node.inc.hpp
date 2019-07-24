#ifndef marlin_ast_node_inc
#define marlin_ast_node_inc

namespace marlin::ast {

namespace utils {

template <typename target, size_t current_index, typename... types>
struct _type_map {};

template <typename target, size_t current_index, typename t0, typename... types>
struct _type_map<target, current_index, t0, types...> {
  static constexpr size_t index{
      _type_map<target, current_index + 1, types...>::index};
};

template <typename target, size_t current_index, typename... types>
struct _type_map<target, current_index, target, types...> {
  static constexpr size_t index{current_index};
};

template <typename target, typename... types>
struct type_map {
  static constexpr size_t index{_type_map<target, 0, types...>::index};
};

}  // namespace utils

#define ASTS(X) X(binary_expression) X(number_literal)

#define LIST_TEMPLATE(NAME) NAME,

template <typename node_type>
inline constexpr size_t node::get_typeid() noexcept {
  return utils::type_map<node_type, ASTS(LIST_TEMPLATE) void>::index;
}

#define SWITCH_CASE_TEMPLATE(NAME) \
  case get_typeid<NAME>():         \
    return callable(as<NAME>());

template <typename return_type, typename callable_type>
inline return_type node::apply(callable_type callable) {
  switch (type()) {
    default:
      /* cannot happen! */
      [[fallthrough]];
      ASTS(SWITCH_CASE_TEMPLATE)
  }
}
template <typename return_type, typename callable_type>
inline return_type node::apply(callable_type callable) const {
  switch (type()) {
    default:
      /* cannot happen! */
      [[fallthrough]];
      ASTS(SWITCH_CASE_TEMPLATE)
  }
}

}  // namespace marlin::ast

#endif  // marlin_ast_node_inc