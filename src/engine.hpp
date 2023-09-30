#pragma once

#include <cstdio>
#include <cstdlib>

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

template <typename T> void free(T *value) { free(value); }

} // namespace memory