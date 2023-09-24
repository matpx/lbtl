#include "renderer.hpp"
#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "sokol/sokol_log.h"
#include "sokol/util/sokol_color.h"

#include "shader/unlit.glsl.h"

namespace renderer {

sg_pipeline unlit_pipeline = {};
sg_bindings quad_bindings = {};

void init() {
  sg_setup({
      .logger =
          {
              .func = slog_func,
          },
      .context = sapp_sgcontext(),
  });

  sg_shader unlit_shader =
      sg_make_shader(unlit_shader_desc(sg_query_backend()));

  sg_pipeline_desc unlit_pipeline_desc = {};
  unlit_pipeline_desc.shader = unlit_shader;
  unlit_pipeline_desc.index_type = SG_INDEXTYPE_UINT16;
  unlit_pipeline_desc.label = "unlit_pipeline";
  unlit_pipeline_desc.layout.attrs[ATTR_vs_position].format =
      SG_VERTEXFORMAT_FLOAT3;

  unlit_pipeline = sg_make_pipeline(unlit_pipeline_desc);

  const float vertices[] = {
      -0.5f, 0.5f,  0.5f, 0.5f,  0.5f,  0.5f,
      0.5f,  -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
  };

  quad_bindings.vertex_buffers[0] = sg_make_buffer(sg_buffer_desc{
      .data = SG_RANGE(vertices),
  });

  const uint16_t indices[] = {0, 1, 2, 0, 2, 3};
  quad_bindings.index_buffer = sg_make_buffer(sg_buffer_desc{
      .type = SG_BUFFERTYPE_INDEXBUFFER,
      .data = SG_RANGE(indices),
  });
}

void frame() {
  sg_pass_action pass_action = {};
  pass_action.colors[0].clear_value = SG_GRAY;

  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());

  sg_apply_pipeline(unlit_pipeline);
  sg_apply_bindings(&quad_bindings);
  sg_draw(0, 6, 1);

  sg_end_pass();
  sg_commit();
}

void finish() { sg_shutdown(); }

} // namespace renderer
