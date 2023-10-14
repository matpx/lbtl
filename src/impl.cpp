#include "alloc.hpp"

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "thirdparty/sokol/sokol_app.h"
#include "thirdparty/sokol/sokol_gfx.h"
#include "thirdparty/sokol/sokol_glue.h"
#include "thirdparty/sokol/sokol_log.h"

#define CGLTF_IMPLEMENTATION
#define CGLTF_MALLOC(size) utils::general_alloc(size)
#define CGLTF_FREE(ptr) utils::general_free(ptr)
#include "thirdparty/cgltf/cgltf.h"

#define STB_DS_IMPLEMENTATION
#define STBDS_REALLOC(context, ptr, size) utils::general_realloc(ptr, size)
#define STBDS_FREE(context, ptr) utils::general_free(ptr)
#include "thirdparty/stb/stb_ds.h"
