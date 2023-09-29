#include "player.hpp"
#include "src/HandmadeMath/HandmadeMath.h"
#include "src/assets.hpp"
#include "src/components.hpp"
#include "src/flecs/flecs.h"
#include "src/renderer.hpp"
#include "world.hpp"

namespace player {

flecs::entity player;
flecs::entity test;

void init() {
  player = world::main.entity().set(comps::Transform{.translation = HMM_V3(0, 0, -20.f)}).set(comps::Camera{});

  world::main.camera = player;

  world::Prefab *prefab = assets::load_model();

  world::main.instantiate(prefab);
}

} // namespace player
