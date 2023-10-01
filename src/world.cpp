#include "world.hpp"
#include "src/HandmadeMath/HandmadeMath.h"
#include "src/components.hpp"
#include "src/engine.hpp"
#include "src/flecs/flecs.h"
#include <cstdint>

namespace world {

World main;
flecs::entity camera;

void World::update() {
  query_transform.each([](const flecs::entity &e, comps::Transform &transform) {
    HMM_Mat4 local = HMM_QToM4(transform.rotation);

    local.Elements[3][0] += transform.translation[0];
    local.Elements[3][1] += transform.translation[1];
    local.Elements[3][2] += transform.translation[2];

    if (transform.parent.is_alive()) {
      transform.world = local * transform.parent.get<comps::Transform>()->world;
    } else {
      transform.world = local;
    }
  });
}

void World::instantiate(const Prefab *prefab) {
  for (int32_t i_node = 0; i_node < arrlen(prefab->nodes); i_node++) {
    const Prefab::Node &node = prefab->nodes[i_node];

    flecs::entity prefab_entity = entity().set(node.transform);

    if (node.has_mesh) {
      prefab_entity.set(prefab->meshbuffer);
      prefab_entity.set(node.mesh);
    }
  }
}

} // namespace world
