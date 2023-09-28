#include "player.hpp"
#include "src/HandmadeMath/HandmadeMath.h"
#include "src/assets.hpp"
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
  // comps::Mesh mesh =
  //     renderer::upload_mesh(SG_RANGE(vertices), SG_RANGE(indices));

  // test = world::main.entity().set(comps::Transform{}).set(mesh);

  player = world::main.entity()
               .set(comps::Transform{.translation = HMM_V3(0, 0, -20.f)})
               .set(comps::Camera{});

  world::main.camera = player;

  assets::load_model();
}

} // namespace player
