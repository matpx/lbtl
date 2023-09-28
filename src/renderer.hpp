#pragma once

#include "src/components.hpp"
#include "src/utils.hpp"
namespace renderer {

void init();

comps::Mesh upload_mesh(const sg_range vertices, const sg_range indices, const size_t index_count);

void frame();

void finish();

} // namespace renderer