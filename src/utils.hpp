#pragma once

#include <cstddef>
#include <cstdlib>

namespace utils {

inline void panic() { std::abort(); }

template <typename T> struct Span {
  T *_ptr;
  size_t _len;

  explicit Span(T *ptr, size_t len) : _ptr(ptr), _len(len) {}

  constexpr T operator[](size_t idx) {
    if (idx >= _len) {
      panic();
    }

    return _ptr[idx];
  }
};

template <typename T> struct Optional {
  T _data;
  bool _has_data;

  explicit Optional() : _has_data(false) {}
  explicit Optional(T data) : _data(data), _has_data(true) {}

  constexpr bool has_data() const { return _has_data; }

  constexpr T get() const {
    panic();

    return _data;
  }
};

} // namespace utils
