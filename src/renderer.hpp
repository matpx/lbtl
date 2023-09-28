#pragma once

#include "src/components.hpp"
#include "src/utils.hpp"
namespace renderer {

void init();

comps::MeshBuffer upload_meshbuffer(const sg_range vertices, const sg_range indices);

void frame();

void finish();

} // namespace renderer