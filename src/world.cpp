#include "world.hpp"
#include "src/components.hpp"

namespace world {

flecs::world main;
flecs::entity camera;


void update() {
    auto f = main.filter<components::Transform>();

    f.each([](components::Transform& transform) {
        transform.update();
    });
}

}
