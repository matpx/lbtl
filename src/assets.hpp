#pragma once

#include "world.hpp"
#include "engine.hpp"

namespace assets {

Result load_model(const char *path, world::Prefab *&out_prefab);

void finish();

}
