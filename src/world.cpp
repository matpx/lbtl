#include "world.hpp"
#include "src/components.hpp"

namespace world {

World main;
flecs::entity camera;

void World::update() {
  query_transform.each(
      [](comps::Transform &transform) { transform.update(); });
}

} // namespace world
