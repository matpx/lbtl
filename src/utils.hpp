#pragma once

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>

namespace utils {

inline void log(const char *prefix, const char *message) {
  printf("[%s] %s\n", prefix, message);
}

inline void panic() {
  log("PANIC", "aborting!");
  std::abort();
}

template <typename T> struct Span {
  T *_ptr;
  size_t _len;

  explicit Span(T *ptr, size_t len) : _ptr(ptr), _len(len) {}

  [[nodiscard]] constexpr T operator[](size_t idx) {
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

  [[nodiscard]] constexpr bool has_data() const { return _has_data; }

  inline void then(
      std::function<void(T)> data, std::function<void()> nodata = [] {}) const {
    if (_has_data) {
      data(_data);
    } else {
      nodata();
    }
  }
};

namespace result {

template <typename T> [[nodiscard]] constexpr Optional<T> ok(T data) {
  return Optional<T>(data);
}

template <typename T>
[[nodiscard]] constexpr Optional<T> error(const char *message) {
  log("ERROR", message);
  return Optional<T>();
}

} // namespace result

} // namespace utils
