#pragma once

#include "src/components.hpp"
#include "src/utils.hpp"
namespace renderer {

void init();

components::Mesh
upload_mesh(const sg_range vertices, const sg_range indices);

void frame();

void finish();

} // namespace renderer