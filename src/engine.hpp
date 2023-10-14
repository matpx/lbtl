#pragma once

#include "linalg.hpp"
#include "thirdparty/stb/stb_ds.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

// primitives

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

using c8 = char;
using c16 = char16_t;

using size = ptrdiff_t;
using usize = size_t;
using uptr = uintptr_t;

// logging

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

// allocator

constexpr void *general_alloc(usize size) {
  constexpr usize alignment = 16; // biggest alignment of any type

  size--;
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  size++;

#ifdef _WIN32
  return _aligned_malloc(size, alignment);
#else
  return aligned_alloc(alignment, size);
#endif
}

constexpr void general_free(void *value) {
#ifdef _WIN32
  return _aligned_free(value);
#else
  free(value);
#endif
}

// pointer

template <typename T> struct NonOwner;

template <typename T> struct Owner {
private:
  T *_value = nullptr;

public:
  Owner() = default;
  explicit Owner(T *value) : _value(value){};

  Owner(const Owner &) = delete;

  Owner &operator=(Owner &&other) {
    this->_value = other._value;
    other._value = nullptr;
    return *this;
  };
  Owner(Owner &&other) { *this = other; };

  static Owner make() {
    T *value = (T *)general_alloc(sizeof(T));

    memset(value, 0, sizeof(T));

    return Owner(value);
  }

  void release() {
    general_free(_value);
    _value = nullptr;
  }

  [[nodiscard]] constexpr T *get() { return _value; }

  T &operator*() const { return _value; }
  T *operator->() const { return _value; }

  friend struct NonOwner<T>;
};

template <typename T> struct NonOwner {
private:
  T *_value = nullptr;

public:
  NonOwner() = default;
  explicit NonOwner(const Owner<T> &value) : _value(value._value){};
  explicit NonOwner(T *value) : _value(value){};

  NonOwner(const NonOwner &) = delete;

  NonOwner &operator=(NonOwner &&other) {
    this->_value = other._value;
    other._value = nullptr;
    return *this;
  };
  NonOwner(NonOwner &&other) { *this = other; };

  void reset() { _value = nullptr; }

  [[nodiscard]] constexpr T *get() { return _value; }

  T &operator*() const { return _value; }
  T *operator->() const { return _value; }
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

// std_ds wrapper

template <typename T> struct DSArray {
private:
  T *_ds_arr = nullptr;

public:
  constexpr T *data() { return _ds_arr; }

  constexpr T &operator[](const usize i) {
    assert(i < arrlenu(_ds_arr));
    return _ds_arr[i];
  }

  void setlen(const usize new_len) { arrsetlen(_ds_arr, new_len); }
  usize len() const { return arrlen(_ds_arr); }

  void push_back(T &&item) { arrpush(_ds_arr, std::move(item)); }

  void release() { arrfree(_ds_arr); }

  DSArray() = default;
  ~DSArray() { assert(_ds_arr == nullptr); };
  DSArray(const DSArray<T> &) = delete;
  DSArray(DSArray<T> &&) = delete;
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
  constexpr Item *data() { return _ds_shmap; }

  Item *get_or_null(const c8 *key) { return shgetp_null(_ds_shmap, key); }

  void put(const c8 *key, V &&value) { shput(_ds_shmap, key, std::move(value)); }

  void release() { shfree(_ds_shmap); }

  DSStringMap() = default;
  ~DSStringMap() { assert(_ds_shmap == nullptr); };
  DSStringMap(const DSStringMap<V> &) = delete;
  DSStringMap(DSStringMap<V> &&) = delete;
};

} // namespace utils