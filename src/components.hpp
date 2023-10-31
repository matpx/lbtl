#pragma once

#include "engine.hpp"
#include "thirdparty/sokol/sokol_gfx.h"

namespace reactphysics3d {

class RigidBody;

}

namespace comps {

struct Transform {
  HMM_Vec3 translation = HMM_V3(0.0f, 0.0f, 0.0f);
  HMM_Quat rotation = HMM_Q(0.0f, 0.0f, 0.0f, 1.0f);

  HMM_Mat4 world;
};

struct RigidBody {
  utils::NonOwner<reactphysics3d::RigidBody> _rigidbody;

  float linear_damping = 0.0f;
  float angular_damping = 0.0f;
};

struct Camera {
  HMM_Mat4 projection;

  Camera() = default;

  Camera(float FOV, float AspectRatio, float Near, float Far) {
    projection = HMM_Perspective_RH_ZO(FOV, AspectRatio, Near, Far);
  }
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

struct Player {
  HMM_Vec2 head_angles;
};

} // namespace comps
