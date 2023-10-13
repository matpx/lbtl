#include "assets.hpp"
#include "engine.hpp"
#include "renderer.hpp"
#include "src/world.hpp"
#include "thirdparty/cgltf/cgltf.h"

namespace assets {

utils::DSArray<utils::Owner<world::Prefab>> prefabs;

utils::Result parse_prim(const cgltf_primitive &gltf_prim, utils::DSArray<comps::MeshBuffer::Vertex> &vertices,
                         utils::DSArray<comps::MeshBuffer::IndexType> &indices, comps::Mesh &out_mesh) {
  cgltf_attribute position_attrib = {};
  cgltf_attribute normal_attrib = {};
  cgltf_attribute texcoord_attrib = {};

  for (cgltf_size i = 0; i < gltf_prim.attributes_count; i++) {
    cgltf_attribute &attrib = gltf_prim.attributes[i];

    if (attrib.type == cgltf_attribute_type_position) {
      position_attrib = attrib;
    } else if (attrib.type == cgltf_attribute_type_normal) {
      normal_attrib = attrib;
    } else if (attrib.type == cgltf_attribute_type_texcoord) {
      texcoord_attrib = attrib;
    } else {
      continue;
    }

    if (attrib.data->is_sparse != 0) {
      return utils::Result::error("attrib.data->is_sparse != 0");
    }
  }

  if (position_attrib.type != cgltf_attribute_type_position || normal_attrib.type != cgltf_attribute_type_normal ||
      texcoord_attrib.type != cgltf_attribute_type_texcoord) {
    return utils::Result::error("gltf attibute missing");
  }

  const usize last_vertices_len = arrlen(vertices.get());
  const usize new_vertices_len = last_vertices_len + position_attrib.data->count;

  if (new_vertices_len >= std::numeric_limits<comps::MeshBuffer::IndexType>::max()) {
    return utils::Result::error("new_vertices_len > std::numeric_limits<int>::max()");
  }

  if (position_attrib.data->count != normal_attrib.data->count ||
      position_attrib.data->count != texcoord_attrib.data->count) {
    return utils::Result::error(
        "pos_attrib.data->count != normal_attrib.data->count || pos_attrib.data->count != uv_attrib.data->count");
  }

  arrsetlen(vertices.get(), new_vertices_len);

  for (cgltf_size i_component = 0; i_component < position_attrib.data->count; i_component++) {
    comps::MeshBuffer::Vertex vertex;

    constexpr usize tmp_count = 3;
    f32 tmp[tmp_count];

    assert(cgltf_accessor_read_float(position_attrib.data, i_component, tmp, tmp_count));
    vertex.position[0] = tmp[0];
    vertex.position[1] = tmp[1];
    vertex.position[2] = tmp[2];

    assert(cgltf_accessor_read_float(normal_attrib.data, i_component, tmp, tmp_count));
    vertex.normal[0] = tmp[0];
    vertex.normal[1] = tmp[1];
    vertex.normal[2] = tmp[2];

    assert(cgltf_accessor_read_float(texcoord_attrib.data, i_component, tmp, tmp_count));
    vertex.uv[0] = tmp[0];
    vertex.uv[1] = tmp[1];

    vertices[last_vertices_len + i_component] = vertex;
  }

  const cgltf_accessor *index_access = gltf_prim.indices;

  const usize last_indices_len = arrlen(indices.get());
  const usize new_indices_len = last_indices_len + index_access->count;

  arrsetlen(indices.get(), new_indices_len);

  for (cgltf_size i_index = 0; i_index < index_access->count; i_index++) {
    comps::MeshBuffer::IndexType index;

    index = cgltf_accessor_read_index(index_access, i_index);

    indices[last_indices_len + i_index] = index;
  }

  out_mesh = {.base_vertex = (comps::MeshBuffer::IndexType)last_indices_len,
              .index_count = (comps::MeshBuffer::IndexType)index_access->count};

  return utils::Result::ok();
}

world::Prefab::Node parse_node(const cgltf_node *gltf_node, utils::DSStringMap<comps::Mesh> &mesh_map) {
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
    const utils::DSStringMap<comps::Mesh>::Item *mesh_kv = shgetp_null(mesh_map.get(), gltf_node->mesh->name);

    if (mesh_kv) {
      node.mesh = mesh_kv->value;
      node.has_mesh = true;
    }
  }

  return node;
}

utils::Result load_model(const char *path, utils::NonOwner<world::Prefab> &out_prefab) {
  out_prefab.reset();

  cgltf_options options = {};
  cgltf_data *data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);

  if (result != cgltf_result_success) {
    return utils::Result::error("can't open gltf file");
  }

  result = cgltf_load_buffers(&options, data, path);

  if (result != cgltf_result_success) {
    cgltf_free(data);

    return utils::Result::error("can't open gltf buffers");
  }

  utils::DSArray<comps::MeshBuffer::Vertex> vertices;
  utils::DSArray<comps::MeshBuffer::IndexType> indices;

  comps::MeshBuffer meshbuffer = {};

  utils::DSStringMap<comps::Mesh> mesh_map;

  for (cgltf_size i_mesh = 0; i_mesh < data->meshes_count; i_mesh++) {
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

  utils::Owner<world::Prefab> prefab = utils::Owner<world::Prefab>::make();
  prefab->meshbuffer = meshbuffer;

  for (cgltf_size i_node = 0; i_node < data->scene->nodes_count; i_node++) {
    arrpush(prefab->nodes, parse_node(data->scene->nodes[i_node], mesh_map));
  }

  shfree(mesh_map.get());
  cgltf_free(data);

  out_prefab = utils::NonOwner<world::Prefab>(prefab);

  arrpush(prefabs.get(), std::move(prefab));

  return utils::Result::ok();
}

void finish() {
  for (i32 i_prefab = 0; i_prefab < arrlen(prefabs.get()); i_prefab++) {
    utils::Owner<world::Prefab> &prefab = prefabs[i_prefab];

    prefab->release();
    prefab.release();
  }

  arrfree(prefabs.get());
}

} // namespace assets
