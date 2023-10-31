#pragma once

#include "types.hpp"

#define STBDS_REALLOC(context, ptr, size) utils::aligned_realloc_16(ptr, size)
#define STBDS_FREE(context, ptr) utils::aligned_free_16(ptr)

namespace utils {

// allocator

void *aligned_alloc_16(usize size);

void aligned_free_16(void *value);

void *aligned_realloc_16(void *old_memory, usize size);

void assert_no_leaks();

} // namespace utils