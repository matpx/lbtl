#pragma once

#include "components.hpp"
#include "flecs/flecs.h"

namespace world {

struct World : public flecs::world {

  flecs::query<comps::Transform> query_transform =
      query<comps::Transform>();

  flecs::query<const comps::Transform, const comps::Mesh>
      query_transform_mesh =
          query<const comps::Transform, const comps::Mesh>();

  void update();
};

extern World main;
extern flecs::entity camera;

} // namespace world
