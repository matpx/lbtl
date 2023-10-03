#pragma once

#include "reactphysics3d/reactphysics3d.h"

namespace physics {

void init();

void update();

void finish();

extern reactphysics3d::PhysicsWorld *world;

}
