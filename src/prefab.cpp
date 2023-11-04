#include "prefab.hpp"
#include "renderer.hpp"

namespace assets {

void Prefab::release() {
  renderer::release_meshbuffer(meshbuffer);
  arrfree(nodes);
}

} // namespace assets