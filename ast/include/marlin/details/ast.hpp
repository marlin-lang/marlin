#ifndef marlin_ast_ast_hpp
#define marlin_ast_ast_hpp

#include "base.hpp"
#include "node.hpp"
#include "specs.hpp"

namespace marlin::ast {

// Forward declarations
struct statement;
struct expression;

struct erroneous_line : base {
  std::string line;

  [[nodiscard]] inline bool is_valid_child(const node &, size_t) const {
    return false;
  }

  explicit inline erroneous_line(std::string _line)
      : base{0}, line{std::move(_line)} {}
};

struct program : base {
  [[nodiscard]] inline size_t statement_count() const {
    return children_count();
  }
  [[nodiscard]] inline node &statement(size_t index) { return child(index); }
  [[nodiscard]] inline const node &statement(size_t index) const {
    return child(index);
  }

  [[nodiscard]] inline bool is_valid_child(const node &n, size_t) const {
    return n.inherits<ast::statement>();
  }

  explicit inline program(utils::move_vector<node> _statements)
      : base{std::move(_statements)} {}
};

struct statement : base {
  using base::base;
};

struct expression_statement : statement {
  [[nodiscard]] inline node &expression() { return child(0); }
  [[nodiscard]] inline const node &expression() const { return child(0); }

  [[nodiscard]] inline bool is_valid_child(const node &n, size_t) const {
    return n.inherits<ast::expression>();
  }

  explicit inline expression_statement(node _expression) : statement{1} {
    push_child(std::move(_expression));
  }
};

struct expression : base {
  using base::base;
};

struct unary_expression : expression {
  unary_op op;

  [[nodiscard]] inline node &argument() { return child(0); }
  [[nodiscard]] inline const node &argument() const { return child(0); }

  [[nodiscard]] inline bool is_valid_child(const node &n, size_t) const {
    return n.inherits<expression>();
  }

  explicit inline unary_expression(unary_op _op, node _argument)
      : expression{1}, op{_op} {
    push_child(std::move(_argument));
  }
};

struct binary_expression : expression {
  binary_op op;

  [[nodiscard]] inline node &left() { return child(0); }
  [[nodiscard]] inline const node &left() const { return child(0); }
  [[nodiscard]] inline node &right() { return child(1); }
  [[nodiscard]] inline const node &right() const { return child(1); }

  [[nodiscard]] inline bool is_valid_child(const node &n, size_t) const {
    return n.inherits<expression>();
  }

  explicit inline binary_expression(node _left, binary_op _op, node _right)
      : expression{2}, op{_op} {
    push_child(std::move(_left));
    push_child(std::move(_right));
  }
};

struct call_expression : expression {
  [[nodiscard]] inline node &callee() { return child(0); }
  [[nodiscard]] inline const node &callee() const { return child(0); }

  [[nodiscard]] inline size_t argument_count() const {
    return children_count() - 1;
  }
  [[nodiscard]] inline node &argument(size_t index) { return child(index + 1); }
  [[nodiscard]] inline const node &argument(size_t index) const {
    return child(index + 1);
  }

  [[nodiscard]] inline bool is_valid_child(const node &n, size_t) const {
    return n.inherits<expression>();
  }

  explicit inline call_expression(node _callee,
                                  utils::move_vector<node> _arguments)
      : expression{_arguments.size() + 1} {
    push_child(std::move(_callee));
    for (node &arg : _arguments) {
      push_child(std::move(arg));
    }
  }
};

struct identifier : expression {
  std::string name;

  [[nodiscard]] inline bool is_valid_child(const node &, size_t) const {
    return false;
  }

  explicit inline identifier(std::string _name)
      : expression{0}, name{std::move(_name)} {}
};

struct bool_literal : expression {
  bool value;

  [[nodiscard]] inline bool is_valid_child(const node &, size_t) const {
    return false;
  }

  explicit inline bool_literal(bool _value) : expression{0}, value{_value} {}
};

// TODO: ast/storage for different forms number literal
// etc. decimal form, scientific form,  etc.
struct number_literal : expression {
  std::string number;

  [[nodiscard]] inline bool is_valid_child(const node &, size_t) const {
    return false;
  }

  explicit inline number_literal(std::string _number)
      : expression{0}, number{std::move(_number)} {}
};

struct string_literal : expression {
  std::string string;

  [[nodiscard]] inline bool is_valid_child(const node &, size_t) const {
    return false;
  }

  explicit inline string_literal(std::string _string)
      : expression{0}, string{std::move(_string)} {}
};

}  // namespace marlin::ast

#endif  // marlin_ast_ast_hpp