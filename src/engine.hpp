#pragma once

#include <cstdio>
#include <cassert>
#include "stb/stb_ds.h"
#include "linalg.hpp"

template<typename T>
struct DSArray {
  T* _internal = nullptr;

  constexpr T*& get() {
    return _internal;
  }

  constexpr T& operator[](const size_t i) {
    assert(i < arrlenu(_internal));
    return _internal[i];
  }

  DSArray() = default;
  DSArray(const DSArray<T>&) = delete;
  DSArray(DSArray<T>&&) = delete;
};

template<typename K, typename V>
struct DSMap {

  struct Item {
    K key;
    V value;
  };

  Item* _internal = nullptr;

  constexpr Item*& get() {
    return _internal;
  }

  DSMap() = default;
  DSMap(const DSMap<K, V>&) = delete;
  DSMap(DSMap<K, V>&&) = delete;
};

template<typename V>
struct DSStringMap {

  struct Item {
    const char* key;
    V value;
  };

  Item* _internal = nullptr;

  constexpr Item*& get() {
    return _internal;
  }

  DSStringMap() = default;
  DSStringMap(const DSStringMap<V>&) = delete;
  DSStringMap(DSStringMap<V>&&) = delete;
};

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

namespace results {

#define RESULT [[nodiscard]] bool

constexpr bool ok() { return true; }

inline bool error(const char *message) {
  LOG_ERROR("%s", message)

  return false;
}

} // namespace results

namespace memory {

template <typename T> T *make() {
  T *value = (T *)malloc(sizeof(T));

  memset(value, 0, sizeof(T));

  return value;
}

template <typename T> void release(T *value) { free(value); }

} // namespace memory
