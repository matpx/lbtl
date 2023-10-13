#pragma once

#include "world.hpp"
#include "engine.hpp"

namespace assets {

utils::Result load_model(const char *path, utils::NonOwner<world::Prefab> &out_prefab);

void finish();

}
