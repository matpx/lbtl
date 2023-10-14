#include "alloc.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <malloc.h>
#include <sys/syslog.h>

namespace utils {

static i32 alloc_counter = 0;

// allocator

void *general_alloc(usize size) {
  constexpr usize alignment = 16; // biggest alignment of any type

  alloc_counter++;

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

void general_free(void *value) {
  if (value == nullptr) {
    return;
  }

  alloc_counter--;

#ifdef _WIN32
  return _aligned_free(value);
#else
  free(value);
#endif
}

void *general_realloc(void *old_memory, usize size) {
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

  // #ifdef _WIN32
  //   return _aligned_realloc(value, size, alignment);
  // #else
  void *new_memory = nullptr;

  if (size != 0) {
    new_memory = general_alloc(size);

    if (old_memory) {
      const usize copy_size = std::min(malloc_usable_size(old_memory), size);

      memcpy(new_memory, old_memory, copy_size);
    }
  }
  general_free(old_memory);

  return new_memory;
  // #endif
}

void assert_no_leaks() { assert(alloc_counter == 0); }

} // namespace utils
