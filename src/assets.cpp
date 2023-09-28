#include "assets.hpp"
#include "cgltf/cgltf.h"
#include "HandmadeMath/HandmadeMath.h"
#include "components.hpp"
#include "renderer.hpp"
#include "utils.hpp"
#include "stb/stb_ds.h"
#include "world.hpp"
#include <cassert>
#include <cstdint>
#include <limits>

namespace assets {

struct Prefab {
  struct Node {
    comps::Transform transform;
    utils::Optional<comps::Mesh> mesh;
  };

  comps::MeshBuffer meshbuffer = {};
  Node *nodes = nullptr;

  void release() { arrfree(nodes); }
};

void load_model() {
  const char *path = "../assets/glb/ships.glb";

  cgltf_options options = {};
  cgltf_data *data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);

  if (result == cgltf_result_success) {

    result = cgltf_load_buffers(&options, data, path);

    if (result == cgltf_result_success) {

      comps::MeshBuffer::Vertex *vertices = nullptr;
      comps::MeshBuffer::IndexType *indices = nullptr;

      comps::MeshBuffer meshbuffer = {};

      struct MeshMapKV {
        const char *key;
        comps::Mesh value;
      };
      MeshMapKV *mesh_map = nullptr;

      for (int32_t i_mesh = 0; i_mesh < data->meshes_count; i_mesh++) {
        const cgltf_mesh *gltf_mesh = &data->meshes[i_mesh];

        for (int32_t i_prim = 0; i_prim < gltf_mesh->primitives_count; i_prim++) {
          const cgltf_primitive &gltf_prim = gltf_mesh->primitives[i_prim];

          if (gltf_prim.attributes_count < 1) {
            LOG_PANIC("prim.attributes_count < 1");
          }

          const cgltf_attribute &pos_attrib = gltf_prim.attributes[0];

          if (pos_attrib.type != cgltf_attribute_type_position) {
            LOG_PANIC("pos_attrib.type != cgltf_attribute_type_position");
          }

          if (pos_attrib.data->is_sparse != 0) {
            LOG_PANIC("pos_attrib.data->is_sparse != 0");
          }

          const size_t last_vertices_len = arrlen(vertices);
          const size_t new_vertices_len = last_vertices_len + pos_attrib.data->count;

          if (new_vertices_len >= std::numeric_limits<comps::MeshBuffer::IndexType>::max()) {
            LOG_PANIC("new_vertices_len > std::numeric_limits<int>::max()");
          }

          arrsetlen(vertices, new_vertices_len);

          for (int32_t i_component = 0; i_component < pos_attrib.data->count; i_component++) {
            comps::MeshBuffer::Vertex vertex;

            cgltf_accessor_read_float(pos_attrib.data, i_component, vertex.poitions,
                                      sizeof(vertex.poitions) / sizeof(vertex.poitions[0]));

            vertices[last_vertices_len + i_component] = vertex;
          }

          const cgltf_accessor *index_access = gltf_prim.indices;

          const size_t last_indices_len = arrlen(indices);
          const size_t new_indices_len = last_indices_len + index_access->count;

          arrsetlen(indices, new_indices_len);

          for (int32_t i_index = 0; i_index < index_access->count; i_index++) {
            comps::MeshBuffer::IndexType index;

            index = cgltf_accessor_read_index(index_access, i_index);

            indices[last_indices_len + i_index] = index;
          }

          comps::Mesh mesh = {.base_vertex = (comps::MeshBuffer::IndexType)last_indices_len,
                              .index_count = (comps::MeshBuffer::IndexType)index_access->count};

          shput(mesh_map, gltf_mesh->name, mesh);
        }
      }

      meshbuffer = renderer::upload_meshbuffer(
          sg_range{.ptr = vertices, .size = arrlenu(vertices) * sizeof(comps::MeshBuffer::Vertex)},
          sg_range{.ptr = indices, .size = arrlenu(indices) * sizeof(comps::MeshBuffer::IndexType)});

      arrfree(vertices);
      arrfree(indices);

      for (int32_t i_node = 0; i_node < data->scene->nodes_count; i_node++) {
        const cgltf_node *node = data->scene->nodes[i_node];

        comps::Transform transform = {};

        if (node->has_translation) {
          transform.translation = HMM_V3(node->translation[0], node->translation[1], node->translation[2]);
        }

        if (node->has_rotation) {
          transform.rotation = HMM_Q(node->rotation[0], node->rotation[1], node->rotation[2], node->rotation[3]);
        }

        auto test_entity = world::main.entity().set(transform).set(meshbuffer);

        if (node->mesh) {
          MeshMapKV *mesh_kv = shgetp_null(mesh_map, node->mesh->name);

          if (mesh_kv)
            test_entity.set(mesh_kv->value);
        }
      }

      shfree(mesh_map);
    }

    cgltf_free(data);
  }
}

} // namespace assets
