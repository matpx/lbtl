#include "physics.hpp"
#include "reactphysics3d/engine/PhysicsCommon.h"
#include "components.hpp"
#include "thirdparty/HandmadeMath/HandmadeMath.h"
#include "world.hpp"

namespace physics {

static reactphysics3d::PhysicsCommon physicsCommon;
reactphysics3d::PhysicsWorld *world = nullptr;

void init() {
  reactphysics3d::PhysicsWorld::WorldSettings settings;

  settings.gravity = reactphysics3d::Vector3(0, 0, 0);

  world = physicsCommon.createPhysicsWorld(settings);

  world::main.observer<comps::Transform, comps::RigidBody>()
      .event(flecs::OnSet)
      .each([](comps::Transform &transform, comps::RigidBody &rigidbody) {
        HMM_Vec3 &comp_translation = transform.translation;
        HMM_Quat &comp_rotation = transform.rotation;

        rigidbody._rigidbody = world->createRigidBody(reactphysics3d::Transform(
            reactphysics3d::Vector3(comp_translation.X, comp_translation.Y, comp_translation.Z),
            reactphysics3d::Quaternion(comp_rotation.X, comp_rotation.Y, comp_rotation.Z, comp_rotation.W)));

        rigidbody._rigidbody->setLinearDamping(rigidbody.linear_damping);
        rigidbody._rigidbody->setAngularDamping(rigidbody.angular_damping);
      });
}

void update(const float delta_time) {
  world->update(delta_time);

  world::main.query_transform_rigidbody.each([](comps::Transform &transform, comps::RigidBody &rigidbody) {
    const reactphysics3d::Vector3 &react_position = rigidbody._rigidbody->getTransform().getPosition();
    const reactphysics3d::Quaternion &react_orientation = rigidbody._rigidbody->getTransform().getOrientation();

    transform.translation = HMM_V3(react_position.x, react_position.y, react_position.z);
    transform.rotation = HMM_Q(react_orientation.x, react_orientation.y, react_orientation.z, react_orientation.w);
  });
}

void finish() { physicsCommon.destroyPhysicsWorld(world); }

} // namespace physics
