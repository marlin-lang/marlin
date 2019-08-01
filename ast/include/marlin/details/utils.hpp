#ifndef marlin_ast_utils_hpp
#define marlin_ast_utils_hpp

#include <utility>
#include <vector>

#include <jsast/jsast.hpp>

namespace marlin {

using jsast::source_loc;
using jsast::source_range;

namespace utils {

using jsast::utils::move_vector;

template <typename vector>
struct _bak_vector_view {
  using value_type = typename vector::value_type;
  using size_type = typename vector::size_type;
  using reference = typename vector::reference;
  using pointer = typename vector::pointer;
  using iterator = typename vector::iterator;

  inline _bak_vector_view(vector& vec, size_type offset = 0)
      : _vec{vec}, _offset{offset} {}

  [[nodiscard]] reference at(size_type pos) const {
    return _vec.at(_offset + pos);
  }
  [[nodiscard]] reference operator[](size_type pos) const {
    return _vec[_offset + pos];
  }
  [[nodiscard]] reference front() const { return *(_vec.begin() + _offset); }
  [[nodiscard]] reference back() const { return _vec.back(); }
  [[nodiscard]] pointer data() const noexcept { return _vec.data() + _offset; }

  [[nodiscard]] iterator begin() const noexcept {
    return _vec.begin() + _offset;
  }
  [[nodiscard]] iterator end() const noexcept { return _vec.end(); }

  [[nodiscard]] bool empty() const noexcept { return _vec.size() <= _offset; }
  [[nodiscard]] size_type size() const noexcept {
    return _vec.size() - _offset;
  }

  template <class... arg_type>
  void emplace(size_type pos, arg_type&&... args) const {
    _vec.emplace(_vec.begin() + _offset + pos, std::forward<arg_type>(args)...);
  }

  value_type pop(size_type pos) const {
    auto it = _vec.begin() + _offset + pos;
    auto item = std::move(*it);
    _vec.erase(it);
    return item;
  }

  template <class... arg_type>
  reference emplace_back(arg_type&&... args) const {
    return _vec.emplace_back(std::forward<arg_type>(args)...);
  }

 private:
  vector& _vec;
  size_type _offset;
};

template <typename vector>
struct _bak_const_vector_view {
  using value_type = typename vector::value_type;
  using size_type = typename vector::size_type;
  using const_reference = typename vector::const_reference;
  using const_pointer = typename vector::const_pointer;
  using const_iterator = typename vector::const_iterator;

  inline _bak_const_vector_view(const vector& vec, size_type offset = 0)
      : _vec{vec}, _offset{offset} {}

  [[nodiscard]] const_reference at(size_type pos) const {
    return _vec.at(_offset + pos);
  }
  [[nodiscard]] const_reference operator[](size_type pos) const {
    return _vec[_offset + pos];
  }
  [[nodiscard]] const_reference front() const {
    return *(_vec.begin() + _offset);
  }
  [[nodiscard]] const_reference back() const { return _vec.back(); }
  [[nodiscard]] const_pointer data() const noexcept {
    return _vec.data() + _offset;
  }

  [[nodiscard]] const_iterator begin() const noexcept {
    return _vec.begin() + _offset;
  }
  [[nodiscard]] const_iterator end() const noexcept { return _vec.end(); }

  [[nodiscard]] bool empty() const noexcept { return _vec.size() <= _offset; }
  [[nodiscard]] size_type size() const noexcept {
    return _vec.size() - _offset;
  }

 private:
  const vector& _vec;
  size_type _offset;
};

template <typename vector>
struct vector_view {
  using value_type = typename vector::value_type;
  using size_type = typename vector::size_type;
  using reference = typename vector::reference;
  using pointer = typename vector::pointer;
  using iterator = typename vector::iterator;

  inline vector_view(vector& vec) : _vec{vec} {}

  [[nodiscard]] reference at(size_type pos) const { return _vec.at(pos); }
  [[nodiscard]] reference operator[](size_type pos) const { return _vec[pos]; }

  [[nodiscard]] iterator begin() const noexcept { return _vec.begin(); }
  [[nodiscard]] iterator end() const noexcept { return _vec.end(); }
  [[nodiscard]] reference front() const { return _vec.front(); }
  [[nodiscard]] reference back() const { return _vec.back(); }
  [[nodiscard]] pointer data() const noexcept { return _vec.data(); }

  [[nodiscard]] bool empty() const noexcept { return _vec.empty(); }
  [[nodiscard]] size_type size() const noexcept { return _vec.size(); }

 private:
  vector& _vec;
};

}  // namespace utils

}  // namespace marlin

#endif  // marlin_ast_utils_hpp