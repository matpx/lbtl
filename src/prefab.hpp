#pragma once

#include "components.hpp"

namespace assets {

struct Prefab {
  struct Node {
    comps::Transform transform;

    bool has_mesh = false;
    comps::Mesh mesh;
  };

  comps::MeshBuffer meshbuffer = {};
  utils::DSArray<Node> nodes;

  void release();
};

}
