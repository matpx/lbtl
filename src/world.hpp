#pragma once

#include "components.hpp"
#include "prefab.hpp"
#include "renderer.hpp"
#include "thirdparty/flecs/flecs.h"


namespace world {

struct World : public flecs::world {

  flecs::query<comps::Transform> query_transform = query<comps::Transform>();

  flecs::query<comps::Transform, const comps::Transform> query_transform_transform =
      query_builder<comps::Transform, const comps::Transform>().parent().cascade().build();

  flecs::query<comps::Transform, comps::RigidBody> query_transform_rigidbody =
      query<comps::Transform, comps::RigidBody>();

  flecs::query<const comps::Transform, const comps::MeshBuffer, const comps::Mesh> query_transform_meshbuffer_mesh =
      query<const comps::Transform, const comps::MeshBuffer, const comps::Mesh>();

  flecs::entity camera;

  void update();

  [[nodiscard]] flecs::entity instantiate(const utils::NonOwner<assets::Prefab> &prefab);
};

extern World main;

} // namespace world
