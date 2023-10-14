#pragma once

#include "types.hpp"

#define CGLTF_MALLOC(size) utils::general_alloc(size)
#define CGLTF_FREE(ptr) utils::general_free(ptr)

#define STBDS_REALLOC(context, ptr, size) utils::general_realloc(ptr, size)
#define STBDS_FREE(context, ptr) utils::general_free(ptr)

namespace utils {

// allocator

void *general_alloc(usize size);

void general_free(void *value);

void *general_realloc(void *old_memory, usize size);

void assert_no_leaks();

} // namespace utils