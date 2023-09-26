#include "player.hpp"
#include "src/components.hpp"
#include "src/flecs/flecs.h"
#include "world.hpp"

namespace player {

flecs::entity player;

void init() {
  player = world::main.entity()
               .set(components::Transform{})
               .set(components::Camera{});
}

} // namespace player
