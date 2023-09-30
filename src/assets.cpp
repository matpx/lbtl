#include "assets.hpp"
#include "HandmadeMath/HandmadeMath.h"
#include "cgltf/cgltf.h"
#include "components.hpp"
#include "renderer.hpp"
#include "src/engine.hpp"
#include "src/flecs/flecs.h"
#include "src/world.hpp"
#include "stb/stb_ds.h"
#include <cstdint>
#include <limits>

namespace assets {

struct MeshMapKV {
  const char *key;
  comps::Mesh value;
};

DSArray<world::Prefab*> prefabs;

RESULT parse_prim(const cgltf_primitive &gltf_prim, DSArray<comps::MeshBuffer::Vertex> &vertices,
                  DSArray<comps::MeshBuffer::IndexType> &indices, comps::Mesh &out_mesh) {
  if (gltf_prim.attributes_count < 1) {
    return results::error("prim.attributes_count < 1");
  }

  const cgltf_attribute &pos_attrib = gltf_prim.attributes[0];

  if (pos_attrib.type != cgltf_attribute_type_position) {
    return results::error("pos_attrib.type != cgltf_attribute_type_position");
  }

  if (pos_attrib.data->is_sparse != 0) {
    return results::error("pos_attrib.data->is_sparse != 0");
  }

  const size_t last_vertices_len = arrlen(vertices.get());
  const size_t new_vertices_len = last_vertices_len + pos_attrib.data->count;

  if (new_vertices_len >= std::numeric_limits<comps::MeshBuffer::IndexType>::max()) {
    return results::error("new_vertices_len > std::numeric_limits<int>::max()");
  }

  arrsetlen(vertices.get(), new_vertices_len);

  for (int32_t i_component = 0; i_component < pos_attrib.data->count; i_component++) {
    comps::MeshBuffer::Vertex vertex;

    cgltf_accessor_read_float(pos_attrib.data, i_component, vertex.poitions,
                              sizeof(vertex.poitions) / sizeof(vertex.poitions[0]));

    vertices[last_vertices_len + i_component] = vertex;
  }

  const cgltf_accessor *index_access = gltf_prim.indices;

  const size_t last_indices_len = arrlen(indices.get());
  const size_t new_indices_len = last_indices_len + index_access->count;

  arrsetlen(indices.get(), new_indices_len);

  for (int32_t i_index = 0; i_index < index_access->count; i_index++) {
    comps::MeshBuffer::IndexType index;

    index = cgltf_accessor_read_index(index_access, i_index);

    indices[last_indices_len + i_index] = index;
  }

  out_mesh = {.base_vertex = (comps::MeshBuffer::IndexType)last_indices_len,
              .index_count = (comps::MeshBuffer::IndexType)index_access->count};

  return results::ok();
}

world::Prefab::Node parse_node(const cgltf_node *gltf_node, DSMap<MeshMapKV> &mesh_map) {
  comps::Transform transform = {};

  if (gltf_node->has_translation) {
    transform.translation = HMM_V3(gltf_node->translation[0], gltf_node->translation[1], gltf_node->translation[2]);
  }

  if (gltf_node->has_rotation) {
    transform.rotation =
        HMM_Q(gltf_node->rotation[0], gltf_node->rotation[1], gltf_node->rotation[2], gltf_node->rotation[3]);
  }

  world::Prefab::Node node = {
      .transform = transform,
  };

  if (gltf_node->mesh) {
    const MeshMapKV *mesh_kv = shgetp_null(mesh_map.get(), gltf_node->mesh->name);

    if (mesh_kv) {
      node.mesh = mesh_kv->value;
      node.has_mesh = true;
    }
  }

  return node;
}

RESULT load_model(const char *path, world::Prefab *&out_prefab) {
  out_prefab = nullptr;

  cgltf_options options = {};
  cgltf_data *data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);

  if (result != cgltf_result_success) {
    return results::error("can't open gltf file");
  }

  result = cgltf_load_buffers(&options, data, path);

  if (result != cgltf_result_success) {
    cgltf_free(data);

    return results::error("can't open gltf buffers");
  }

  DSArray<comps::MeshBuffer::Vertex> vertices;
  DSArray<comps::MeshBuffer::IndexType> indices;

  comps::MeshBuffer meshbuffer = {};

  DSMap<MeshMapKV> mesh_map;

  for (int32_t i_mesh = 0; i_mesh < data->meshes_count; i_mesh++) {
    const cgltf_mesh *gltf_mesh = &data->meshes[i_mesh];

    if (gltf_mesh->primitives_count > 0) {
      comps::Mesh mesh;

      if (parse_prim(gltf_mesh->primitives[0], vertices, indices, mesh)) {
        shput(mesh_map.get(), gltf_mesh->name, mesh);
      }
    }
  }

  meshbuffer = renderer::upload_meshbuffer(
      sg_range{.ptr = vertices.get(), .size = arrlenu(vertices.get()) * sizeof(comps::MeshBuffer::Vertex)},
      sg_range{.ptr = indices.get(), .size = arrlenu(indices.get()) * sizeof(comps::MeshBuffer::IndexType)});

  arrfree(vertices.get());
  arrfree(indices.get());

  world::Prefab *prefab = memory::make<world::Prefab>();
  prefab->meshbuffer = meshbuffer;

  for (int32_t i_node = 0; i_node < data->scene->nodes_count; i_node++) {
    arrpush(prefab->nodes, parse_node(data->scene->nodes[i_node], mesh_map));
  }

  shfree(mesh_map.get());
  cgltf_free(data);

  arrpush(prefabs.get(), prefab);


  out_prefab = prefab;

  return true;
}

void finish() {
  for (int32_t i_prefab = 0; i_prefab < arrlen(prefabs.get()); i_prefab++) {
    prefabs[i_prefab]->release();
  }

  arrfree(prefabs.get());
}

} // namespace assets
