#pragma once

#include "HandmadeMath/HandmadeMath.h"
#include "sokol/sokol_gfx.h"
#include <cstdint>

namespace comps {

struct Transform {
  HMM_Vec3 translation = HMM_V3(0.0f, 0.0f, 0.0f);
  HMM_Quat rotation = HMM_Q(1.0f, 0.0f, 0.0f, 0.0f);

  HMM_Mat4 world;

  constexpr void update() {
    world = HMM_Translate(translation) * HMM_QToM4(rotation); // TODO faster
  }
};

struct Camera {
  HMM_Mat4 projection;

  Camera() { projection = HMM_Perspective_RH_NO(0.125f, 1.0f, 0.1f, 1000.0f); }
};

struct Mesh {

  struct Vertex {
    float poitions[3];
  };

  using IndexType = uint16_t;

  sg_pipeline pipeline;
  sg_bindings bindings;

  size_t index_count;
};

} // namespace comps
