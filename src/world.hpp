#pragma once

#include "components.hpp"
#include "flecs/flecs.h"

namespace world {

struct World : public flecs::world {

  flecs::filter<comps::Transform> filter_transform =
      filter<comps::Transform>();

  flecs::filter<const comps::Transform, const comps::Mesh>
      filter_transform_mesh =
          filter<const comps::Transform, const comps::Mesh>();

  void update();
};

extern World main;
extern flecs::entity camera;

} // namespace world
