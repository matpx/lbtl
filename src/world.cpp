#include "world.hpp"

namespace world {

World main;
flecs::entity camera;

void World::update() {
  query_transform.each([](comps::Transform &transform) {
    HMM_Mat4 local = HMM_QToM4(transform.rotation);

    HMM_TranslateInplace(local, transform.translation);

    if (transform.parent.is_alive()) {
      transform.world = transform.parent.get<comps::Transform>()->world * local;
    } else {
      transform.world = local;
    }
  });
}

void World::instantiate(const Prefab *prefab) {
  for (i32 i_node = 0; i_node < arrlen(prefab->nodes); i_node++) {
    const Prefab::Node &node = prefab->nodes[i_node];

    flecs::entity prefab_entity = entity().set(node.transform);

    if (node.has_mesh) {
      prefab_entity.set(prefab->meshbuffer);
      prefab_entity.set(node.mesh);
    }
  }
}

} // namespace world
