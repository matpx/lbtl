#include "world.hpp"

namespace world {

World main;
flecs::entity camera;

void World::update() {
  query_transform.each([](comps::Transform &transform) {
    transform.world = HMM_QToM4(transform.rotation);
    HMM_TranslateInplace(transform.world, transform.translation);
  });

  query_transform_transform.each([](comps::Transform &transform, const comps::Transform &parent_transform) {
    transform.world = parent_transform.world * transform.world;
  });
}

void World::instantiate(const Prefab *prefab) {
  const flecs::entity base = entity().set(prefab->meshbuffer);

  for (i32 i_node = 0; i_node < arrlen(prefab->nodes); i_node++) {
    const Prefab::Node &node = prefab->nodes[i_node];

    flecs::entity prefab_entity = entity().set(node.transform);

    if (node.has_mesh) {
      prefab_entity.is_a(base);
      prefab_entity.set(node.mesh);
    }
  }
}

} // namespace world
