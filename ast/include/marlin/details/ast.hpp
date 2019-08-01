#ifndef marlin_ast_ast_hpp
#define marlin_ast_ast_hpp

#include "base.hpp"
#include "node.hpp"
#include "specs.hpp"

namespace marlin::ast {

using const_nodes_view = utils::const_vector_view<utils::move_vector<node>>;
using nodes_view = utils::vector_view<utils::move_vector<node>>;

// Forward declarations
struct statement {};
struct expression {};

struct erroneous_line : base::impl<erroneous_line> {
  std::string line;

  [[nodiscard]] inline bool is_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline erroneous_line(std::string _line) : line{std::move(_line)} {}
};

struct program : base::impl<program> {
  [[nodiscard]] inline nodes_view statements() { return {_children}; }
  [[nodiscard]] inline const_nodes_view statements() const {
    return {_children};
  }

  [[nodiscard]] inline bool is_valid_child(const base &n, size_t) const {
    return n.inherits<ast::statement>();
  }

  explicit inline program(utils::move_vector<node> _statements) {
    for (auto &statement : _statements) {
      push_back_child(std::move(statement));
    }
  }
};

struct expression_statement : base::impl<expression_statement>, statement {
  [[nodiscard]] inline node &expression() { return _children[0]; }
  [[nodiscard]] inline const node &expression() const { return _children[0]; }

  [[nodiscard]] inline bool is_valid_child(const base &n, size_t) const {
    return n.inherits<ast::expression>();
  }

  explicit inline expression_statement(node _expression) {
    push_back_child(std::move(_expression));
  }
};

struct unary_expression : base::impl<unary_expression>, expression {
  unary_op op;

  [[nodiscard]] inline node &argument() { return _children[0]; }
  [[nodiscard]] inline const node &argument() const { return _children[0]; }

  [[nodiscard]] inline bool is_valid_child(const base &n, size_t) const {
    return n.inherits<expression>();
  }

  [[nodiscard]] inline source_range op_range() const noexcept {
    const auto start{source_code_range().begin};
    const auto length{length_of(op)};
    return {start, {start.line, start.column + length}};
  }

  explicit inline unary_expression(unary_op _op, node _argument) : op{_op} {
    push_back_child(std::move(_argument));
  }
};

struct binary_expression : base::impl<binary_expression>, expression {
  binary_op op;

  [[nodiscard]] inline node &left() { return _children[0]; }
  [[nodiscard]] inline const node &left() const { return _children[0]; }
  [[nodiscard]] inline node &right() { return _children[1]; }
  [[nodiscard]] inline const node &right() const { return _children[1]; }

  [[nodiscard]] inline bool is_valid_child(const base &n, size_t) const {
    return n.inherits<expression>();
  }

  [[nodiscard]] inline source_range op_range() const noexcept {
    const auto length{length_of(op)};
    return {_op_loc, {_op_loc.line, _op_loc.column + length}};
  }

  explicit inline binary_expression(node _left, binary_op _op,
                                    source_loc _op_start, node _right)
      : op{_op}, _op_loc{_op_start} {
    push_back_child(std::move(_left));
    push_back_child(std::move(_right));
  }

 private:
  source_loc _op_loc;
};

struct call_expression : base::impl<call_expression>, expression {
  [[nodiscard]] inline node &callee() { return _children[0]; }
  [[nodiscard]] inline const node &callee() const { return _children[0]; }

  [[nodiscard]] inline nodes_view arguments() { return {_children, 1}; }
  [[nodiscard]] inline const_nodes_view arguments() const {
    return {_children, 1};
  }

  [[nodiscard]] inline bool is_valid_child(const base &n, size_t) const {
    return n.inherits<expression>();
  }

  explicit inline call_expression(node _callee,
                                  utils::move_vector<node> _arguments) {
    push_back_child(std::move(_callee));
    for (auto &argument : _arguments) {
      push_back_child(std::move(argument));
    }
  }
};

struct identifier : base::impl<identifier>, expression {
  std::string name;

  [[nodiscard]] inline bool is_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline identifier(std::string _name) : name{std::move(_name)} {}
};

struct bool_literal : base::impl<bool_literal>, expression {
  bool value;

  [[nodiscard]] inline bool is_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline bool_literal(bool _value) : value{_value} {}
};

// TODO: ast/storage for different forms number literal
// etc. decimal form, scientific form,  etc.
struct number_literal : base::impl<number_literal>, expression {
  std::string number;

  [[nodiscard]] inline bool is_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline number_literal(std::string _number)
      : number{std::move(_number)} {}
};

struct string_literal : base::impl<string_literal>, expression {
  std::string string;

  [[nodiscard]] inline bool is_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline string_literal(std::string _string)
      : string{std::move(_string)} {}
};

}  // namespace marlin::ast

#endif  // marlin_ast_ast_hpp