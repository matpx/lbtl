#pragma once

#include "HandmadeMath/HandmadeMath.h"
#include "sokol/sokol_gfx.h"
#include "src/engine.hpp"
#include "src/flecs/flecs.h"
#include <cstdint>

namespace comps {

struct Transform {
  flecs::entity parent = flecs::entity::null();

  HMM_Vec3 translation = HMM_V3(0.0f, 0.0f, 0.0f);
  HMM_Quat rotation = HMM_Q(0.0f, 0.0f, 0.0f, 1.0f);

  HMM_Mat4 world;
};

struct Camera {
  HMM_Mat4 projection;

  Camera() { projection = HMM_Perspective_RH_ZO(0.125f, 1.0f, 0.1f, 1000.0f); }
};

struct MeshBuffer {

  struct Vertex {
    float poitions[3];
  };

  using IndexType = uint16_t;

  sg_pipeline pipeline;
  sg_bindings bindings;
};

struct Mesh {
  MeshBuffer::IndexType base_vertex;
  MeshBuffer::IndexType index_count;
};

} // namespace comps
