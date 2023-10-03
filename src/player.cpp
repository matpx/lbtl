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
                    .set(comps::Transform{.translation = HMM_V3(0.0f, 0.0f, 0.f)})
                    .set(comps::RigidBody{
                        ._rigidbody = physics::world->createRigidBody(reactphysics3d::Transform{}),
                    });
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
  // auto &rotation = player_root.get_mut<comps::Transform>()->rotation;
  // rotation = HMM_MulQ(HMM_QFromAxisAngle_RH(HMM_V3(0, 1, 0), 0.002f), rotation);

  const HMM_Vec2 &axis = input::get_left_axis();
  reactphysics3d::RigidBody *rb = player_root.get_mut<comps::RigidBody>()->_rigidbody;

  rb->applyLocalForceAtCenterOfMass(reactphysics3d::Vector3(axis.X, 0, -axis.Y) * 10);
}

} // namespace player
