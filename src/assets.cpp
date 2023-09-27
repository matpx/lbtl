#include "assets.hpp"
#include "cgltf/cgltf.h"
#include "src/utils.hpp"
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

          for (int32_t i_component = 0; i_component < pos_attrib.data->count; i_component++) {
            float component_value[3];

            assert(cgltf_accessor_read_float(pos_attrib.data, i_component, component_value,
                                             sizeof(component_value) / sizeof(float)));

            LOG_INFO("%f", component_value[0]);
          }
        }
      }
    }

    cgltf_free(data);
  }
}

} // namespace assets
