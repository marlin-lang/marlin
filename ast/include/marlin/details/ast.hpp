#ifndef marlin_ast_ast_hpp
#define marlin_ast_ast_hpp

#include "base.hpp"
#include "node.hpp"
#include "specs.hpp"

namespace marlin::ast {

struct expression : base {
  using base::base;
};

struct unary_expression : expression {
  unary_op op;

  inline node& argument() { return _children[0]; }
  inline const node& argument() const { return _children[0]; }

  explicit inline unary_expression(unary_op _op, node _argument)
      : expression{1}, op{_op} {
    _children.emplace_back(std::move(_argument));
  }
};

struct binary_expression : expression {
  binary_op op;

  inline node& left() { return _children[0]; }
  inline const node& left() const { return _children[0]; }
  inline node& right() { return _children[1]; }
  inline const node& right() const { return _children[1]; }

  explicit inline binary_expression(node _left, binary_op _op, node _right)
      : expression{2}, op{_op} {
    _children.emplace_back(std::move(_left));
    _children.emplace_back(std::move(_right));
  }
};

// TODO: ast/storage for different forms number literal
// etc. decimal form, scientific form,  etc.
struct number_literal : expression {
  std::string number;

  explicit inline number_literal(std::string _number)
      : expression{0}, number{std::move(_number)} {}
};

}  // namespace marlin::ast

#endif  // marlin_ast_ast_hpp