#pragma once

#include "linalg.hpp"
#include "thirdparty/stb/stb_ds.h"
#include <cassert>
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

// stb ds wrapper

template <typename T> struct DSArray {
  T *_internal = nullptr;

  constexpr T *&get() { return _internal; }

  constexpr T &operator[](const size_t i) {
    assert(i < arrlenu(_internal));
    return _internal[i];
  }

  DSArray() = default;
  DSArray(const DSArray<T> &) = delete;
  DSArray(DSArray<T> &&) = delete;
};

template <typename K, typename V> struct DSMap {

  struct Item {
    K key;
    V value;
  };

  Item *_internal = nullptr;

  constexpr Item *&get() { return _internal; }

  DSMap() = default;
  DSMap(const DSMap<K, V> &) = delete;
  DSMap(DSMap<K, V> &&) = delete;
};

template <typename V> struct DSStringMap {

  struct Item {
    const char *key;
    V value;
  };

  Item *_internal = nullptr;

  constexpr Item *&get() { return _internal; }

  DSStringMap() = default;
  DSStringMap(const DSStringMap<V> &) = delete;
  DSStringMap(DSStringMap<V> &&) = delete;
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

constexpr void *general_alloc(size_t size) {
  constexpr size_t alignment = 16; // biggest alignment of any type

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
