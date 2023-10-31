#include "alloc.hpp"

#define STB_DS_IMPLEMENTATION
#include "thirdparty/stb/stb_ds.h"

#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "thirdparty/sokol/sokol_app.h"
#include "thirdparty/sokol/sokol_gfx.h"
#include "thirdparty/sokol/sokol_glue.h"
#include "thirdparty/sokol/sokol_log.h"

#define CGLTF_MALLOC(size) utils::aligned_alloc_16(size)
#define CGLTF_FREE(ptr) utils::aligned_free_16(ptr)
#define CGLTF_IMPLEMENTATION
#include "thirdparty/cgltf/cgltf.h"
