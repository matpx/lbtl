#pragma once

#include "components.hpp"
#include "renderer.hpp"
#include "thirdparty/flecs/flecs.h"

namespace world {

struct Prefab {
  struct Node {
    comps::Transform transform;

    bool has_mesh = false;
    comps::Mesh mesh;
  };

  comps::MeshBuffer meshbuffer = {};
  Node *nodes = nullptr;

  void release() {
    renderer::release_meshbuffer(meshbuffer);
    arrfree(nodes);
  }
};

struct World : public flecs::world {

  flecs::query<comps::Transform> query_transform = query<comps::Transform>();

  flecs::query<comps::Transform, const comps::Transform> query_transform_transform =
      query_builder<comps::Transform, const comps::Transform>().term_at(2).parent().cascade().build();

  flecs::query<comps::Transform, comps::RigidBody> query_transform_rigidbody =
      query<comps::Transform, comps::RigidBody>();

  flecs::query<const comps::Transform, const comps::MeshBuffer, const comps::Mesh> query_transform_meshbuffer_mesh =
      query<const comps::Transform, const comps::MeshBuffer, const comps::Mesh>();

  flecs::entity camera;

  void update();

  void instantiate(const Prefab *prefab);
};

extern World main;

} // namespace world
