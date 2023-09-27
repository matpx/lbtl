#include "player.hpp"
#include "src/HandmadeMath/HandmadeMath.h"
#include "src/components.hpp"
#include "src/flecs/flecs.h"
#include "src/renderer.hpp"
#include "world.hpp"

namespace player {

flecs::entity player;
flecs::entity test;

const float vertices[] = {
    -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
};

const uint16_t indices[] = {0, 1, 2, 0, 2, 3};

void init() {
  components::Mesh mesh =
      renderer::upload_mesh(SG_RANGE(vertices), SG_RANGE(indices));

  test = world::main.entity().set(components::Transform{}).set(mesh);

  player = world::main.entity()
               .set(components::Transform{.translation = HMM_V3(0, 0, -10.f)})
               .set(components::Camera{});

  world::camera = player;
}

} // namespace player
