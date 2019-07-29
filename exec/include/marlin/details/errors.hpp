#ifndef marlin_exec_errors_hpp
#define marlin_exec_errors_hpp

#include <stdexcept>

#include <marlin/ast.hpp>

namespace marlin::exec {

enum class error_type { generation, runtime };

struct generation_error : std::exception {
  inline generation_error(std::string message, ast::base& node)
      : _message{std::move(message)}, _node{&node} {}

  const char* what() const noexcept override { return _message.data(); }

  inline ast::base& node() noexcept { return *_node; }

 private:
  std::string _message;
  ast::base* _node;
};

struct runtime_error : std::exception {
  inline runtime_error(std::string message) : _message{std::move(message)} {}

  const char* what() const noexcept override { return _message.data(); }

 private:
  std::string _message;
};

};  // namespace marlin::exec

#endif  // marlin_parse_errors_hpp