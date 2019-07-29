#ifndef marlin_parse_errors_hpp
#define marlin_parse_errors_hpp

#include <stdexcept>

#include <marlin/ast.hpp>

namespace marlin::parse {

struct filesystem_error : std::exception {
  inline filesystem_error(std::string message) : _message{message} {}

  const char* what() const noexcept override { return _message.data(); }

 private:
  std::string _message;
};

struct error : std::exception {
  friend struct interpreter;

  inline error(std::string message, source_loc loc)
      : _message{message}, _loc{loc} {}

  const char* what() const noexcept override { return _message.data(); }

  inline source_loc loc() const noexcept { return _loc; }
  inline ast::base& node() noexcept { return *_node; }

 private:
  std::string _message;
  source_loc _loc;
  ast::base* _node{nullptr};

  inline void set_node(ast::base& node) { _node = &node; }
};

};  // namespace marlin::parse

#endif  // marlin_parse_errors_hpp