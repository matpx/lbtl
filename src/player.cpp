#include "player.hpp"
#include "assets.hpp"
#include "components.hpp"
#include "src/engine.hpp"
#include "src/input.hpp"
#include "src/physics.hpp"
#include "src/renderer.hpp"
#include "src/thirdparty/HandmadeMath/HandmadeMath.h"
#include "src/thirdparty/flecs/flecs.h"
#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Vector3.h>

namespace player {

flecs::entity player_root;
flecs::entity player_head;

void init() {
  const HMM_Vec2 width_height = renderer::get_width_height();

  player_root = world::main.entity()
                    .set(comps::Transform{.translation = HMM_V3(0.0, 0.0, 0.0)})
                    .set(comps::RigidBody{});
  player_head = world::main.entity()
                    .set(comps::Transform{.translation = HMM_V3(0.0, 2.0, 10.0)})
                    .set(comps::Camera(0.25f, width_height.X / width_height.Y, 0.1f, 1000.0f))
                    .child_of(player_root);

  world::main.camera = player_head;

  utils::NonOwner<world::Prefab> prefab;

  if (assets::load_model("./assets/glb/ships.glb", prefab)) {
    flecs::entity space_ship = world::main.instantiate(prefab);
    space_ship.get_mut<comps::Transform>()->translation.X = 0.01;
  }
}

void update() {

  // head rotation

  static HMM_Vec2 head_angles = HMM_V2(0, 0);

  head_angles += input::get_right_axis() * 0.00075;
  head_angles.Y = HMM_Clamp(-0.25, head_angles.Y, 0.25);

  const HMM_Quat vertical_rotation = HMM_QFromAxisAngle_LH(HMM_V3(1, 0, 0), head_angles.Y);
  const HMM_Quat horizontal_rotation = HMM_QFromAxisAngle_LH(HMM_V3(0, 1, 0), head_angles.X);

  player_head.get_mut<comps::Transform>()->rotation = horizontal_rotation * vertical_rotation;

  // root movement

  const HMM_Vec2 left_axis = input::get_left_axis();
  utils::NonOwner<reactphysics3d::RigidBody> rb =
      player_root.get_mut<comps::RigidBody>()->_rigidbody;

  HMM_Vec3 movement_velocity = horizontal_rotation * HMM_V3(left_axis.X, 0, -left_axis.Y);

  rb->setLinearVelocity(
      reactphysics3d::Vector3(movement_velocity.X, movement_velocity.Y, movement_velocity.Z) * 5);
}

} // namespace player
