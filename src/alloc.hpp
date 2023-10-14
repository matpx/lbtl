#pragma once

#include "types.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace utils {

// allocator

constexpr void *general_alloc(usize size) {
  constexpr usize alignment = 16; // biggest alignment of any type

  if (size < alignment) { // TODO wrong
    size = alignment;
  }

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

inline void *general_realloc(void *old_memory, usize size) {
  constexpr usize alignment = 16; // biggest alignment of any type

  if (size < alignment) { // TODO wrong
    size = alignment;
  }

  size--;
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  size++;

#ifdef _WIN32
  return _aligned_realloc(value, size, alignment);
#else
  void *new_memory = aligned_alloc(alignment, size);
  const usize copy_size = std::min(malloc_usable_size(old_memory), size);

  memcpy(new_memory, old_memory, copy_size);

  free(old_memory);

  return new_memory;
#endif
}

} // namespace utils