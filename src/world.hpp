#pragma once

#include "stb/stb_ds.h"
#include "components.hpp"
#include "flecs/flecs.h"

namespace world {

struct Prefab {
  struct Node {
    comps::Transform transform;

    bool has_mesh = false;
    comps::Mesh mesh;
  };

  comps::MeshBuffer meshbuffer = {};
  Node *nodes = nullptr;

  void release() { arrfree(nodes); }
};

struct World : public flecs::world {

  flecs::query<comps::Transform> query_transform = query<comps::Transform>();

  flecs::query<const comps::Transform, const comps::MeshBuffer, const comps::Mesh> query_transform_meshbuffer_mesh =
      query<const comps::Transform, const comps::MeshBuffer, const comps::Mesh>();

  flecs::entity camera;

  void update();

  void instantiate(const Prefab* prefab);
};

extern World main;

} // namespace world
