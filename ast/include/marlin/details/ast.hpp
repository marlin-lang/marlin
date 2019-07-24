#ifndef marlin_ast_ast_hpp
#define marlin_ast_ast_hpp

#include "base.hpp"
#include "node.hpp"
#include "specs.hpp"

namespace marlin::ast {

struct expression : base {
  using base::base;
};

struct binary_expression : expression {
  binary_op op;

  inline node& left() { return _children[0]; }
  inline const node& left() const { return _children[0]; }
  inline node& right() { return _children[1]; }
  inline const node& right() const { return _children[1]; }

  explicit inline binary_expression(node&& left, binary_op _op, node&& right)
      : expression{2}, op{_op} {
    _children.emplace_back(std::move(left));
    _children.emplace_back(std::move(right));
  }
};

struct number_literal : expression {
  double number;

  explicit inline number_literal(double _number)
      : expression{0}, number{_number} {}
};

}  // namespace marlin::ast

#endif  // marlin_ast_ast_hpp