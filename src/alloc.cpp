#include "alloc.hpp"
#include "engine.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <malloc.h>

namespace utils {

static i32 alloc_counter = 0;

constexpr usize alignment = 16;
constexpr usize align_size(const usize size) { return ((size - 1) | (alignment - 1)) + 1; }

void *aligned_alloc_16(usize size) {

  alloc_counter++;

  size = align_size(size);

#ifdef _WIN32
  void *ptr = _aligned_malloc(size, alignment);
#else
  void *ptr = aligned_alloc(alignment, size);
#endif

  // memset(ptr, 0, size);

  return ptr;
}

void aligned_free_16(void *value) {
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

void *aligned_realloc_16(void *old_memory, usize size) {
  size = align_size(size);

#ifdef _WIN32
  if (size != 0) {
    alloc_counter++;
  }
  if (old_memory) {
    alloc_counter--;
  }

  return _aligned_realloc(old_memory, size, alignment);
#else
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
#endif
}

void assert_no_leaks() { LOG_ASSERT(alloc_counter == 0); }

} // namespace utils
