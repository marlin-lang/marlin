#ifndef marlin_parse_infix_builder_hpp
#define marlin_parse_infix_builder_hpp

#include <marlin/ast.hpp>

namespace marlin::parse {

struct interpreter::infix_builder {
  inline infix_builder(interpreter& interp, code node, uint8_t threshold_prec)
      : _interp{interp},
        _node{std::move(node)},
        _threshold_prec{threshold_prec} {}

  inline bool success() { return _success_flag; }

  inline code result() && { return std::move(_node); }

  inline void parse_binary(ast::binary_op op, precedence p) {
    if (test(p)) {
      _interp.next();
      _node = ast::binary_expression{
          std::move(_node), op,
          _interp.parse_precedence(static_cast<uint8_t>(p) + 1)};
    }
  }

 private:
  interpreter& _interp;
  code _node;
  uint8_t _threshold_prec;
  bool _success_flag = true;

  inline bool test(precedence p) { return test(static_cast<uint8_t>(p)); }
  inline bool test(uint8_t p) {
    if (p < _threshold_prec) {
      _success_flag = false;
    }
    return _success_flag;
  }
};

inline interpreter::infix_builder interpreter::make_builder(
    code node, uint8_t threshold_prec) {
  return {*this, std::move(node), threshold_prec};
}

}  // namespace marlin::parse

#endif  // marlin_parse_infix_builder_hpp