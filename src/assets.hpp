#pragma once

#include "world.hpp"
#include "engine.hpp"

namespace assets {

utils::Result load_model(const c8 *path, utils::NonOwner<world::Prefab> &out_prefab);

void finish();

}
