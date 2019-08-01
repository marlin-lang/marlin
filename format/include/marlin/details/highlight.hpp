#ifndef marlin_format_highlight_hpp
#define marlin_format_highlight_hpp

#include <string>

#include <marlin/ast.hpp>

// Testing
#include <iostream>

namespace marlin::format {

struct highlight {
  enum class token_type : uint8_t { op, boolean, number, string };

  struct token {
    token_type type;
    source_range range;

    explicit inline token(token_type _type, source_range _range)
        : type{_type}, range{_range} {}
  };

  inline highlight(const ast::base& c) noexcept : _code{c} {}

  inline std::vector<token> generate() {
    generate_tree(_code);
    auto tokens{std::move(_tokens)};
    _tokens = {};
    return tokens;
  }

 private:
  const ast::base& _code;
  std::vector<token> _tokens;

  void generate_tree(const ast::base& c) {
    if (!c.apply<bool>(
            [this](const auto& node) { return generate_tokens(node); })) {
      for (const auto& child : c.children()) {
        generate_tokens(child);
      }
    }
  }

  inline void generate_tokens(const code& c) { generate_tree(c.get()); }

  template <typename node_type, typename = std::enable_if_t<
                                    std::is_base_of_v<ast::base, node_type>>>
  inline bool generate_tokens(const node_type&) {
    return false;
  }
};

template <>
inline bool highlight::generate_tokens(const ast::unary_expression& unary) {
  _tokens.emplace_back(token_type::op, unary.op_range());
  generate_tokens(unary.argument());
  return true;
}

template <>
inline bool highlight::generate_tokens(const ast::binary_expression& binary) {
  generate_tokens(binary.left());
  _tokens.emplace_back(token_type::op, binary.op_range());
  generate_tokens(binary.right());
  return true;
}

template <>
inline bool highlight::generate_tokens(const ast::bool_literal& literal) {
  _tokens.emplace_back(token_type::boolean, literal.source_code_range());
  return true;
}

template <>
inline bool highlight::generate_tokens(const ast::number_literal& literal) {
  _tokens.emplace_back(token_type::number, literal.source_code_range());
  return true;
}

template <>
inline bool highlight::generate_tokens(const ast::string_literal& literal) {
  _tokens.emplace_back(token_type::string, literal.source_code_range());
  return true;
}

}  // namespace marlin::format

#endif  // marlin_format_highlight_hpp