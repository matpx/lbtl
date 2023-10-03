#pragma once

#include "engine.hpp"
#include "thirdparty/flecs/flecs.h"
#include "thirdparty/sokol/sokol_gfx.h"

namespace reactphysics3d {

class RigidBody;

}

namespace comps {

struct Transform {
  flecs::entity parent = flecs::entity::null();

  HMM_Vec3 translation = HMM_V3(0.0f, 0.0f, 0.0f);
  HMM_Quat rotation = HMM_Q(0.0f, 0.0f, 0.0f, 1.0f);

  HMM_Mat4 world;
};

struct RigidBody {
  reactphysics3d::RigidBody *_rigidbody;
};

struct Camera {
  HMM_Mat4 projection;

  Camera() { projection = HMM_Perspective_RH_ZO(0.125f, 1.0f, 0.1f, 1000.0f); }
};

struct MeshBuffer {

  struct Vertex {
    float position[3];
    float normal[3];
    float uv[2];
  };

  using IndexType = u16;

  sg_pipeline pipeline;
  sg_bindings bindings;
};

struct Mesh {
  MeshBuffer::IndexType base_vertex;
  MeshBuffer::IndexType index_count;
};

} // namespace comps
