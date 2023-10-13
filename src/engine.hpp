#pragma once

#include "linalg.hpp"
#include "thirdparty/stb/stb_ds.h"
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <utility>

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

using byte = char;

using size = ptrdiff_t;
using usize = size_t;

// stb ds wrapper

template <typename T> class DSArray {
  T *_data = nullptr;

public:
  DSArray() = default;
  ~DSArray() { arrfree(_data); }
  DSArray(const DSArray<T> &) = delete;
  DSArray(DSArray<T> &&) = delete;

  usize size() const { return arrlen(_data); }

  T *data() { return _data; }

  constexpr T &operator[](const usize i) {
    assert(i < arrlenu(_data));
    return _data[i];
  }

  void push_back(T &&value) { arrpush(_data, std::forward<T>(value)); }

  void resize(usize new_size) { arrsetlen(_data, new_size); }

  void clear() { arrfree(_data); }
};

// template <typename K, typename V> struct DSMap {

//   struct Item {
//     K key;
//     V value;
//   };

//   Item *_internal = nullptr;

//   DSMap() = default;
//   DSMap(const DSMap<K, V> &) = delete;
//   DSMap(DSMap<K, V> &&) = delete;

//   constexpr Item *data() { return _internal; }
// };

template <typename V> struct DSStringMap {

  struct Item {
    const char *key;
    V value;
  };

  Item *_data = nullptr;

  DSStringMap() = default;
  ~DSStringMap() { shfree(_data); };
  DSStringMap(const DSStringMap<V> &) = delete;
  DSStringMap(DSStringMap<V> &&) = delete;

  constexpr Item *data() { return _data; }

  Item *get_or_null(const char *key) { return shgetp_null(_data, key); }

  void emplace(const char *key, V &&value) { shput(_data, key, std::forward<V>(value)); }
};

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

// result

struct [[nodiscard]] Result {
  bool _value;

  static constexpr Result ok() { return {._value = true}; }

  static inline Result error(const char *message) {
    LOG_ERROR("%s", message);
    return {._value = false};
  }

  operator bool() const { return _value; }
};

namespace memory {

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

template <typename T> T *make() { // TODO use everywhere
  T *value = (T *)general_alloc(sizeof(T));

  memset(value, 0, sizeof(T));

  return value;
}

template <typename T> void release(T *value) { general_free(value); }

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

  static Owner make() { return Owner(memory::make<T>()); }
  void release() { memory::release(_value); }

  [[nodiscard]] constexpr T *get() { return _value; }

  T &operator*() { return _value; }
  T *operator->() { return _value; }
};

} // namespace memory
