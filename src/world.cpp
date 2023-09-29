#include "world.hpp"
#include "src/components.hpp"
#include "src/flecs/flecs.h"
#include "src/utils.hpp"
#include <cstdint>

namespace world {

World main;
flecs::entity camera;

void World::update() {
  query_transform.each([](comps::Transform &transform) { transform.update(); });
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
