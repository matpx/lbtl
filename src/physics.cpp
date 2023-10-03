#include "physics.hpp"
#include "reactphysics3d/body/CollisionBody.h"
#include "reactphysics3d/body/RigidBody.h"
#include "reactphysics3d/mathematics/Quaternion.h"
#include "reactphysics3d/mathematics/Transform.h"
#include "reactphysics3d/mathematics/Vector3.h"
#include "src/components.hpp"
#include "src/thirdparty/HandmadeMath/HandmadeMath.h"
#include "src/world.hpp"

namespace physics {

static reactphysics3d::PhysicsCommon physicsCommon;
reactphysics3d::PhysicsWorld *world = nullptr;

void init() {
  reactphysics3d::PhysicsWorld::WorldSettings settings;

  settings.gravity = reactphysics3d::Vector3(0, 0, 0);

  world = physicsCommon.createPhysicsWorld(settings);
}

void update() {
  world->update(0.01f);

  world::main.query_transform_rigidbody.each([](comps::Transform &transform, comps::RigidBody &rigidbody) {
    const reactphysics3d::Vector3 &position = rigidbody._rigidbody->getTransform().getPosition();
    const reactphysics3d::Quaternion &orientation = rigidbody._rigidbody->getTransform().getOrientation();

    transform.translation = HMM_V3(position.x, position.y, position.z);
    transform.rotation = HMM_Q(orientation.x, orientation.y, orientation.z, orientation.w);
  });
}

void finish() { physicsCommon.destroyPhysicsWorld(world); }

} // namespace physics
