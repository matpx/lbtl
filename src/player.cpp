#include "player.hpp"
#include "assets.hpp"
#include "components.hpp"

namespace player {

flecs::entity player_root;
flecs::entity player_head;

void init() {
  player_root = world::main.entity().set(comps::Transform{.translation = HMM_V3(0.0f, 0.0f, 0.f)});
  player_head = world::main.entity()
                    .set(comps::Transform{.parent = player_root, .translation = HMM_V3(0.0, 2.0f, 20.0f)})
                    .set(comps::Camera{});

  world::main.camera = player_head;

  world::Prefab *prefab = nullptr;

  if (assets::load_model("./assets/glb/ships.glb", prefab)) {
    world::main.instantiate(prefab);
  }
}

void update() {
  auto &rotation = player_root.get_mut<comps::Transform>()->rotation;

  rotation = HMM_MulQ(HMM_QFromAxisAngle_RH(HMM_V3(0, 1, 0), 0.002f), rotation);
}

} // namespace player
