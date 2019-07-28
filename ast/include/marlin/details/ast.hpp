#ifndef marlin_ast_ast_hpp
#define marlin_ast_ast_hpp

#include "base.hpp"
#include "node.hpp"
#include "specs.hpp"

namespace marlin::ast {

struct program : base {
  inline size_t statement_count() const { return _children.size(); }
  inline node& statement(size_t index) { return _children[index]; }
  inline const node& statement(size_t index) const { return _children[index]; }

  explicit inline program(utils::move_vector<node> _statements)
      : base{std::move(_statements)} {}
};

struct statement : base {
  using base::base;
};

struct expression_statement : statement {
  inline node& expression() { return _children[0]; }
  inline const node& expression() const { return _children[0]; }

  explicit inline expression_statement(node _expression) : statement{1} {
    _children.emplace_back(std::move(_expression));
  }
};

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

struct call_expression : expression {
  inline node& callee() { return _children[0]; }
  inline const node& callee() const { return _children[0]; }

  inline size_t argument_count() const { return _children.size() - 1; }
  inline node& argument(size_t index) { return _children[index + 1]; }
  inline const node& argument(size_t index) const {
    return _children[index + 1];
  }

  explicit inline call_expression(node _callee,
                                  utils::move_vector<node> _arguments)
      : expression{_arguments.size() + 1} {
    _children.emplace_back(std::move(_callee));
    std::move(_arguments.begin(), _arguments.end(),
              std::back_inserter(_children));
  }
};

struct identifier : expression {
  std::string name;

  explicit inline identifier(std::string _name)
      : expression{0}, name{std::move(_name)} {}
};

// TODO: ast/storage for different forms number literal
// etc. decimal form, scientific form,  etc.
struct number_literal : expression {
  std::string number;

  explicit inline number_literal(std::string _number)
      : expression{0}, number{std::move(_number)} {}
};

struct string_literal : expression {
  std::string string;

  explicit inline string_literal(std::string _string)
      : expression{0}, string{std::move(_string)} {}
};

}  // namespace marlin::ast

#endif  // marlin_ast_ast_hpp