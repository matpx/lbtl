#pragma once

#include "flecs/flecs.h"
#include "components.hpp"

namespace world {

void update();

extern flecs::world main;
extern flecs::entity camera;

}
