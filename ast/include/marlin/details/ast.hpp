#ifndef marlin_ast_ast_hpp
#define marlin_ast_ast_hpp

#include "base.hpp"
#include "node.hpp"
#include "specs.hpp"
#include "subnodes.hpp"

namespace marlin::ast {

struct statement {};
struct expression {};

struct placeholder : base::impl<placeholder> {
  std::string name;

  [[nodiscard]] inline bool check_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline placeholder(std::string _name = "")
      : name{std::move(_name)} {}
};

struct erroneous_line : base::impl<erroneous_line> {
  std::string line;

  [[nodiscard]] inline bool check_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline erroneous_line(std::string _line) : line{std::move(_line)} {}
};

struct program : base::impl<program> {
  [[nodiscard]] inline subnode_vector_view statements() {
    return get_subnode(_statements);
  }
  [[nodiscard]] inline const_subnode_vector_view statements() const {
    return get_subnode(_statements);
  }

  [[nodiscard]] inline bool check_valid_child(const base &n, size_t) const {
    return n.inherits<ast::statement>();
  }

  explicit inline program(utils::move_vector<node> _s) {
    init(_statements, std::move(_s));
  }

 private:
  subnode::vector _statements;
};

struct expression_statement : base::impl<expression_statement>, statement {
  [[nodiscard]] inline node &expression() { return get_subnode(_expression); }
  [[nodiscard]] inline const node &expression() const {
    return get_subnode(_expression);
  }

  [[nodiscard]] inline bool check_valid_child(const base &n, size_t) const {
    return n.inherits<ast::expression>();
  }

  explicit inline expression_statement(node _expr) {
    init(_expression, std::move(_expr));
  }

 private:
  subnode::concrete _expression;
};

struct unary_expression : base::impl<unary_expression>, expression {
  unary_op op;

  [[nodiscard]] inline node &argument() { return get_subnode(_argument); }
  [[nodiscard]] inline const node &argument() const {
    return get_subnode(_argument);
  }

  [[nodiscard]] inline bool check_valid_child(const base &n, size_t) const {
    return n.inherits<expression>();
  }

  [[nodiscard]] inline source_range op_range() const noexcept {
    const auto start{source_code_range().begin};
    const auto length{length_of(op)};
    return {start, {start.line, start.column + length}};
  }

  explicit inline unary_expression(unary_op _op, node _arg) : op{_op} {
    init(_argument, std::move(_arg));
  }

 private:
  subnode::concrete _argument;
};

struct binary_expression : base::impl<binary_expression>, expression {
  friend parse::interpreter;

  binary_op op;

  [[nodiscard]] inline node &left() { return get_subnode(_left); }
  [[nodiscard]] inline const node &left() const { return get_subnode(_left); }
  [[nodiscard]] inline node &right() { return get_subnode(_right); }
  [[nodiscard]] inline const node &right() const { return get_subnode(_right); }

  [[nodiscard]] inline bool check_valid_child(const base &n, size_t) const {
    return n.inherits<expression>();
  }

  [[nodiscard]] inline source_range op_range() const noexcept {
    const auto length{length_of(op)};
    return {_op_loc, {_op_loc.line, _op_loc.column + length}};
  }

  explicit inline binary_expression(node _l, binary_op _op, node _r) : op{_op} {
    init(_left, std::move(_l), _right, std::move(_r));
  }

 private:
  subnode::concrete _left;
  subnode::concrete _right;

  source_loc _op_loc;
};

struct call_expression : base::impl<call_expression>, expression {
  [[nodiscard]] inline node &callee() { return get_subnode(_callee); }
  [[nodiscard]] inline const node &callee() const {
    return get_subnode(_callee);
  }

  [[nodiscard]] inline subnode_vector_view arguments() {
    return get_subnode(_arguments);
  }
  [[nodiscard]] inline const_subnode_vector_view arguments() const {
    return get_subnode(_arguments);
  }

  [[nodiscard]] inline bool check_valid_child(const base &n, size_t) const {
    return n.inherits<expression>();
  }

  explicit inline call_expression(node _c, utils::move_vector<node> _arg) {
    init(_callee, std::move(_c), _arguments, std::move(_arg));
  }

 private:
  subnode::concrete _callee;
  subnode::vector _arguments;
};

struct identifier : base::impl<identifier>, expression {
  std::string name;

  [[nodiscard]] inline bool check_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline identifier(std::string _name) : name{std::move(_name)} {}
};

struct bool_literal : base::impl<bool_literal>, expression {
  bool value;

  [[nodiscard]] inline bool check_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline bool_literal(bool _value) : value{_value} {}
};

// TODO: ast/storage for different forms number literal
// etc. decimal form, scientific form,  etc.
struct number_literal : base::impl<number_literal>, expression {
  std::string number;

  [[nodiscard]] inline bool check_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline number_literal(std::string _number)
      : number{std::move(_number)} {}
};

struct string_literal : base::impl<string_literal>, expression {
  std::string string;

  [[nodiscard]] inline bool check_valid_child(const base &, size_t) const {
    return false;
  }

  explicit inline string_literal(std::string _string)
      : string{std::move(_string)} {}
};

}  // namespace marlin::ast

#endif  // marlin_ast_ast_hpp
