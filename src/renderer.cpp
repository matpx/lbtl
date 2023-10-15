#include "renderer.hpp"
#include "src/thirdparty/HandmadeMath/HandmadeMath.h"
#include "thirdparty/sokol/sokol_app.h"
#include "thirdparty/sokol/sokol_glue.h"
#include "thirdparty/sokol/sokol_log.h"
#include "thirdparty/sokol/util/sokol_color.h"

#include "shader/unlit.glsl.h"
#include "world.hpp"

namespace renderer {

sg_pipeline unlit_pipeline = {};

void init() {
  const static auto my_alloc = [](size_t size, [[maybe_unused]] void *user_data) -> void * {
    return utils::general_alloc(size);
  };

  const static auto my_free = [](void *ptr, [[maybe_unused]] void *user_data) { utils::general_free(ptr); };

  sg_setup({
      .allocator =
          {
              .alloc = my_alloc,
              .free = my_free,
          },
      .logger =
          {
              .func = slog_func,
          },
      .context = sapp_sgcontext(),
  });

  if (!sg_isvalid()) {
    LOG_PANIC("!sg_isvalid()")
  }

  sg_shader unlit_shader = sg_make_shader(unlit_shader_desc(sg_query_backend()));

  sg_pipeline_desc unlit_pipeline_desc = {};
  unlit_pipeline_desc.shader = unlit_shader;
  unlit_pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
  unlit_pipeline_desc.label = "unlit_pipeline";
  unlit_pipeline_desc.layout.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3;
  unlit_pipeline_desc.layout.attrs[ATTR_vs_normal0].format = SG_VERTEXFORMAT_FLOAT3;
  unlit_pipeline_desc.layout.attrs[ATTR_vs_uv0].format = SG_VERTEXFORMAT_FLOAT2;
  unlit_pipeline_desc.depth = {.compare = SG_COMPAREFUNC_LESS_EQUAL, .write_enabled = true},

  unlit_pipeline = sg_make_pipeline(unlit_pipeline_desc);
}

comps::MeshBuffer upload_meshbuffer(const sg_range vertices, const sg_range indices) {
  comps::MeshBuffer mesh = {};

  mesh.bindings.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc{.data = vertices});

  mesh.bindings.index_buffer = sg_make_buffer(sg_buffer_desc{
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .data = indices,
  });

  return mesh;
}

void release_meshbuffer(comps::MeshBuffer &meshbuffer) {
  sg_destroy_buffer(meshbuffer.bindings.index_buffer);
  sg_destroy_buffer(meshbuffer.bindings.vertex_buffers[0]);
}

void draw() {
  sg_pass_action pass_action = {};
  pass_action.colors[0].clear_value = SG_GRAY;

  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());

  sg_apply_pipeline(unlit_pipeline);

  const HMM_Mat4 view = HMM_InvGeneral(world::main.camera.get<comps::Transform>()->world);
  const HMM_Mat4 proj = world::main.camera.get<comps::Camera>()->projection;

  const HMM_Mat4 vp = proj * view;

  world::main.query_transform_meshbuffer_mesh.each(
      [&](const comps::Transform &transform, const comps::MeshBuffer &meshbuffer, const comps::Mesh &mesh) {
        const HMM_Mat4 mvp = vp * transform.world;

        const vs_params_t vs_params = {
            .mvp = mvp,
        };

        sg_apply_bindings(&meshbuffer.bindings);
        sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));

        sg_draw(mesh.base_vertex, mesh.index_count, 1);
      });

  sg_end_pass();
  sg_commit();
}

void finish() { sg_shutdown(); }

[[nodiscard]] HMM_Vec2 get_width_height() { return HMM_V2(sapp_widthf(), sapp_heightf()); }

} // namespace renderer
