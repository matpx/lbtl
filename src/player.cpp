#include "player.hpp"
#include "src/HandmadeMath/HandmadeMath.h"
#include "src/assets.hpp"
#include "src/components.hpp"
#include "src/flecs/flecs.h"
#include "src/renderer.hpp"
#include "world.hpp"

namespace player {

flecs::entity player_root;
flecs::entity player_head;

void init() {
  player_root = world::main.entity().set(comps::Transform{.translation = HMM_V3(0.0f, 0.0f, 30.f)});
  player_head = world::main.entity()
                    .set(comps::Transform{.parent = player_root, .translation = HMM_V3(0.0, 2.0f, 0.0f)})
                    .set(comps::Camera{});

  world::main.camera = player_head;

  world::Prefab *prefab = nullptr;

  if (assets::load_model("./assets/glb/ships.glb", prefab)) {
    world::main.instantiate(prefab);
  }
}

void update() {
  // player_root.get_mut<comps::Transform>()->translation.X += 0.025f;
}

} // namespace player
