#pragma once

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <functional>

namespace utils {

#define LOG_INFO(...) { printf("[INFO] "); printf(__VA_ARGS__); printf("\n"); }
#define LOG_ERROR(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); }
#define LOG_PANIC(...) { printf("[PANIC] "); printf(__VA_ARGS__); printf("\n"); std::abort(); }

template <typename T> struct Span {
  T *_ptr;
  size_t _len;

  explicit Span(T *ptr, size_t len) : _ptr(ptr), _len(len) {}

  [[nodiscard]] constexpr T operator[](size_t idx) {
    if (idx >= _len) {
      LOG_PANIC("Span out of bounds!");
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
  LOG_ERROR(message);
  return Optional<T>();
}

} // namespace result

} // namespace utils
