#pragma once

#include "engine.hpp"
#include "prefab.hpp"
#include "world.hpp"


namespace assets {

utils::Result load_model(const c8 *path, utils::NonOwner<assets::Prefab> &out_prefab);

void finish();

} // namespace assets
