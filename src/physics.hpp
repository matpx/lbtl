#pragma once

#include "reactphysics3d/engine/PhysicsWorld.h"

namespace physics {

void init();

void update(float delta_time);

void finish();

extern reactphysics3d::PhysicsWorld *world;

}
