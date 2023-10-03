#include "player.hpp"
#include "assets.hpp"
#include "components.hpp"
#include "src/input.hpp"
#include "src/physics.hpp"
#include "src/thirdparty/HandmadeMath/HandmadeMath.h"

namespace player {

flecs::entity player_root;
flecs::entity player_head;

void init() {
  player_root = world::main.entity()
                    .set(comps::Transform{.translation = HMM_V3(0.0, 0.0, 20.0)})
                    .set(comps::RigidBody{});
  player_head = world::main.entity()
                    .set(comps::Transform{.parent = player_root, .translation = HMM_V3(0.0, 0.0, 0.0)})
                    .set(comps::Camera{});

  world::main.camera = player_head;

  world::Prefab *prefab = nullptr;

  if (assets::load_model("./assets/glb/ships.glb", prefab)) {
    world::main.instantiate(prefab);
  }
}

void update() {
  const HMM_Vec2 left_axis = input::get_left_axis();
  reactphysics3d::RigidBody *rb = player_root.get_mut<comps::RigidBody>()->_rigidbody;
  rb->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(left_axis.X, 0, -left_axis.Y) * 10);

  const HMM_Vec2 right_axis = input::get_right_axis();
  rb->applyLocalTorque(reactphysics3d::Vector3(-1,0,0) * right_axis.Y * 0.1f + reactphysics3d::Vector3(0,-1,0) * right_axis.X * 0.1f);
}

} // namespace player
