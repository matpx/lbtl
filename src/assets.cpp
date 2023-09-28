#include "assets.hpp"
#include "cgltf/cgltf.h"
#include "src/components.hpp"
#include "src/renderer.hpp"
#include "src/utils.hpp"
#include "stb/stb_ds.h"
#include "world.hpp"
#include <cassert>

namespace assets {

void load_model() {
  const char *path = "../assets/glb/ships.glb";

  cgltf_options options = {};
  cgltf_data *data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);

  if (result == cgltf_result_success) {

    result = cgltf_load_buffers(&options, data, path);

    if (result == cgltf_result_success) {
      for (int32_t i_mesh = 0; i_mesh < data->meshes_count; i_mesh++) {
        const cgltf_mesh &mesh = data->meshes[i_mesh];

        for (int32_t i_prim = 0; i_prim < mesh.primitives_count; i_prim++) {
          const cgltf_primitive &prim = mesh.primitives[i_prim];

          if (prim.attributes_count < 1) {
            LOG_PANIC("prim.attributes_count < 1");
          }

          const cgltf_attribute &pos_attrib = prim.attributes[0];

          if (pos_attrib.type != cgltf_attribute_type_position) {
            LOG_PANIC("pos_attrib.type != cgltf_attribute_type_position");
          }

          if (pos_attrib.data->is_sparse != 0) {
            LOG_PANIC("pos_attrib.data->is_sparse != 0");
          }

          comps::Mesh::Vertex *vertices = nullptr;
          arrsetlen(vertices, pos_attrib.data->count);

          for (int32_t i_component = 0; i_component < pos_attrib.data->count; i_component++) {
            comps::Mesh::Vertex vertex;

            cgltf_accessor_read_float(pos_attrib.data, i_component, vertex.poitions,
                                      sizeof(vertex.poitions) / sizeof(vertex.poitions[0]));

            vertices[i_component] = vertex;
          }

          const cgltf_accessor *index_access = prim.indices;

          comps::Mesh::IndexType *indices = nullptr;
          arrsetlen(indices, index_access->count);

          for (int32_t i_index = 0; i_index < index_access->count; i_index++) {
            comps::Mesh::IndexType index;

            index = cgltf_accessor_read_index(index_access, i_index);

            indices[i_index] = index;
          }

          comps::Mesh mesh = renderer::upload_mesh(
              sg_range{.ptr = vertices, .size = arrlenu(vertices) * sizeof(comps::Mesh::Vertex)},
              sg_range{.ptr = indices, .size = arrlenu(indices) * sizeof(comps::Mesh::IndexType)}, arrlenu(indices));

          world::main.entity().set(comps::Transform{}).set(mesh);
        }
      }
    }

    cgltf_free(data);
  }
}

} // namespace assets
