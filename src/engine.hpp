#pragma once

#include "alloc.hpp"
#include "linalg.hpp"
#include "thirdparty/stb/stb_ds.h"
#include "types.hpp"
#include <cstdio>
#include <cstdlib>
#include <utility>

// logging

// TODO rename logging macros

#define LOG_ASSERT(CONDITION)                                                                                          \
  {                                                                                                                    \
    if (!(CONDITION)) {                                                                                                \
      LOG_PANIC("[%s:%d] Assertion failed", __FILE__, __LINE__);                                                       \
    }                                                                                                                  \
  }

#ifndef NDEBUG
#define LOG_DEBUG(...)                                                                                                 \
  {                                                                                                                    \
    printf("[DEBUG] ");                                                                                                \
    printf(__VA_ARGS__);                                                                                               \
    printf("\n");                                                                                                      \
  }
#else
#define LOG_DEBUG(...)                                                                                                 \
  {}
#endif
#define LOG_INFO(...)                                                                                                  \
  {                                                                                                                    \
    printf("[INFO] ");                                                                                                 \
    printf(__VA_ARGS__);                                                                                               \
    printf("\n");                                                                                                      \
  }
#define LOG_ERROR(...)                                                                                                 \
  {                                                                                                                    \
    printf("[ERROR] ");                                                                                                \
    printf(__VA_ARGS__);                                                                                               \
    printf("\n");                                                                                                      \
  }
#define LOG_PANIC(...)                                                                                                 \
  {                                                                                                                    \
    printf("[PANIC] ");                                                                                                \
    printf(__VA_ARGS__);                                                                                               \
    printf("\n");                                                                                                      \
    std::abort();                                                                                                      \
  }

// utils

namespace utils {

// pointer

template <typename T> struct NonOwner;

template <typename T> struct Owner {
private:
  T *_value = nullptr;

public:
  Owner() = default;
  explicit Owner(T *value) : _value(value){};
  Owner(const Owner &) = delete;
  Owner(Owner &&other) { *this = other; };

  Owner &operator=(Owner &&other) {
    // LOG_ASSERT(_value == nullptr);

    this->_value = other._value;
    other._value = nullptr;
    return *this;
  };

  T &operator*() const { return *_value; }
  T *operator->() const { return _value; }

  static Owner make() {
    T *value = static_cast<T *>(aligned_alloc_16(sizeof(T)));

    memset(value, 0, sizeof(T));

    return Owner(value);
  }

  [[nodiscard]] constexpr T *get() { return _value; }

  void release() {
    aligned_free_16(_value);
    _value = nullptr;
  }

  friend struct NonOwner<T>;
};

template <typename T> struct NonOwner {
private:
  T *_value = nullptr;

public:
  NonOwner() = default;
  explicit NonOwner(const Owner<T> &value) : _value(value._value){};
  explicit NonOwner(T *value) : _value(value){};
  NonOwner(const NonOwner &other) { *this = other; };
  NonOwner(NonOwner &&other) { *this = other; };

  NonOwner &operator=(const NonOwner &other) {
    this->_value = other._value;
    return *this;
  };

  NonOwner &operator=(NonOwner &&other) {
    this->_value = other._value;
    other._value = nullptr;
    return *this;
  };

  NonOwner &operator=(T *ptr) {
    _value = ptr;
    return *this;
  };

  T &operator*() const { return *_value; }
  T *operator->() const { return _value; }

  [[nodiscard]] constexpr T *get() { return _value; }

  void reset() { _value = nullptr; }
};

// result

struct [[nodiscard]] Result {
  bool _value;

  static constexpr Result ok() { return {._value = true}; }

  static inline Result error(const c8 *message) {
    LOG_ERROR("%s", message);
    return {._value = false};
  }

  operator bool() const { return _value; }
};

// optional

template <typename T> struct [[nodiscard]] Optional {
  T _value;
  bool _has_value;

  Optional() : _has_value(false), _value({}) {}
  Optional(T value) : _has_value(true), _value(value) {}

  operator bool() const { return _has_value; }

  T get() const {
    LOG_ASSERT(_has_value);
    return _value;
  }
};

// array

template <typename T, usize N> struct Array {
  T _data[N];

  constexpr static usize SIZE = N;

  constexpr T &operator[](const usize i) {
    LOG_ASSERT(i < N);
    return _data[i];
  }
};

// std_ds wrapper

template <typename T> struct DSArray {
private:
  T *_ds_arr = nullptr;

public:
  DSArray() = default;
  ~DSArray() { LOG_ASSERT(_ds_arr == nullptr); };
  DSArray(const DSArray<T> &) = delete;
  DSArray(DSArray<T> &&) = delete;

  constexpr T *data() { return _ds_arr; }

  constexpr T &operator[](const usize i) {
    LOG_ASSERT(i < arrlenu(_ds_arr));
    return _ds_arr[i];
  }

  usize size() const { return arrlen(_ds_arr); }

  void resize(const usize new_len) { arrsetlen(_ds_arr, new_len); }
  void emplace_back(T &&item) { arrpush(_ds_arr, std::forward<T>(item)); }
  void release() { arrfree(_ds_arr); }
};

// template <typename K, typename V> struct DSMap {

//   struct Item {
//     K key;
//     V value;
//   };

//   Item *_internal = nullptr;

//   constexpr Item *&get() { return _internal; }

//   DSMap() = default;
//   DSMap(const DSMap<K, V> &) = delete;
//   DSMap(DSMap<K, V> &&) = delete;
// };

template <typename V> struct DSStringMap {
public:
  struct Item {
    const c8 *key;
    V value;
  };

private:
  Item *_ds_shmap = nullptr;

public:
  DSStringMap() = default;
  ~DSStringMap() { LOG_ASSERT(_ds_shmap == nullptr); };
  DSStringMap(const DSStringMap<V> &) = delete;
  DSStringMap(DSStringMap<V> &&) = delete;

  constexpr Item *data() { return _ds_shmap; }

  Item *get_or_null(const c8 *key) { return shgetp_null(_ds_shmap, key); }

  void put(const c8 *key, V &&value) { shput(_ds_shmap, key, std::forward<V>(value)); }
  void release() { shfree(_ds_shmap); }
};

} // namespace utils