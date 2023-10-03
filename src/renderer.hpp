#pragma once

#include "components.hpp"

namespace renderer {

void init();

comps::MeshBuffer upload_meshbuffer(const sg_range vertices, const sg_range indices);

void release_meshbuffer(comps::MeshBuffer& meshbuffer);

void draw();

void finish();

} // namespace renderer