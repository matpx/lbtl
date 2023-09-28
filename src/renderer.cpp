#include "renderer.hpp"
#include "flecs/flecs.h"
#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "sokol/sokol_log.h"
#include "sokol/util/sokol_color.h"

#include "HandmadeMath/HandmadeMath.h"
#include "shader/unlit.glsl.h"
#include "src/components.hpp"
#include "src/world.hpp"

namespace renderer {

sg_pipeline unlit_pipeline = {};

void init() {
  sg_setup({
      .logger =
          {
              .func = slog_func,
          },
      .context = sapp_sgcontext(),
  });

  sg_shader unlit_shader = sg_make_shader(unlit_shader_desc(sg_query_backend()));

  sg_pipeline_desc unlit_pipeline_desc = {};
  unlit_pipeline_desc.shader = unlit_shader;
  unlit_pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
  unlit_pipeline_desc.label = "unlit_pipeline";
  unlit_pipeline_desc.layout.attrs[ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3;

  unlit_pipeline = sg_make_pipeline(unlit_pipeline_desc);
}

comps::Mesh upload_mesh(const sg_range vertices, const sg_range indices, const size_t index_count) {
  comps::Mesh mesh = {};

  mesh.bindings.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc{.data = vertices});

  mesh.bindings.index_buffer = sg_make_buffer(sg_buffer_desc{
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .data = indices,
  });

  mesh.index_count = index_count;

  return mesh;
}

void frame() {
  sg_pass_action pass_action = {};
  pass_action.colors[0].clear_value = SG_GRAY;

  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());

  sg_apply_pipeline(unlit_pipeline);

  const HMM_Mat4 view = HMM_InvGeneral(world::main.camera.get<comps::Transform>()->world);
  const HMM_Mat4 proj = world::main.camera.get<comps::Camera>()->projection;

  const HMM_Mat4 vp = proj * view;

  world::main.query_transform_mesh.each([&](const comps::Transform &transform, const comps::Mesh &mesh) {
    const HMM_Mat4 mvp = vp * transform.world;

    const vs_params_t vs_params = {
        .mvp = mvp,
    };

    sg_apply_bindings(&mesh.bindings);
    sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_vs_params, SG_RANGE(vs_params));

    sg_draw(0, mesh.index_count, 1);
  });

  sg_end_pass();
  sg_commit();
}

void finish() { sg_shutdown(); }

} // namespace renderer
