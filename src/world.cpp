#include "world.hpp"
#include "engine.hpp"
#include "thirdparty/flecs/flecs.h"

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

flecs::entity World::instantiate(const utils::NonOwner<assets::Prefab> &prefab) {
  const flecs::entity prefab_root = entity().set(comps::Transform{});

  const flecs::entity base = entity().set(prefab->meshbuffer);

  for (i32 i_node = 0; i_node < prefab->nodes.size(); i_node++) {
    const assets::Prefab::Node &node = prefab->nodes[i_node];

    flecs::entity prefab_entity = entity().set(node.transform).child_of(prefab_root);

    if (node.has_mesh) {
      prefab_entity.is_a(base);
      prefab_entity.set(node.mesh);
    }
  }

  return prefab_root;
}

} // namespace world
