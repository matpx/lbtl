#pragma once

#include "HandmadeMath/HandmadeMath.h"
#include "sokol/sokol_gfx.h"

namespace components {

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

  Camera() { projection = HMM_Perspective_RH_NO(1.0f, 1.0f, 0.1f, 1000.0f); }
};

struct Mesh {
  sg_pipeline pipeline;
  sg_bindings bindings;

  Mesh(sg_pipeline pipeline, sg_bindings bindings)
      : pipeline(pipeline), bindings(bindings) {}
};

} // namespace components
