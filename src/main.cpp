#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol/sokol_app.h"
#include "sokol/sokol_gfx.h"
#include "sokol/sokol_glue.h"
#include "sokol/sokol_log.h"
#include "sokol/util/sokol_color.h"

static void init(void) {
  sg_setup({
      .logger =
          {
              .func = slog_func,
          },
      .context = sapp_sgcontext(),
  });
}

void event(const sapp_event *event) {
  if (event->type == SAPP_EVENTTYPE_KEY_DOWN &&
      event->key_code == SAPP_KEYCODE_ESCAPE) {
    sapp_request_quit();
  }
}

void frame(void) {
  sg_pass_action pass_action = {};
  pass_action.colors[0].clear_value = SG_BLUE;

  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
  sg_end_pass();
  sg_commit();
}

void cleanup(void) { sg_shutdown(); }

sapp_desc sokol_main(int argc, char *argv[]) {
  return (sapp_desc){.init_cb = init,
                     .frame_cb = frame,
                     .cleanup_cb = cleanup,
                     .event_cb = event,
                     .width = 1200,
                     .height = 800,
                     .window_title = "lbtl",
                     .icon =
                         {
                             .sokol_default = true,
                         },
                     .logger = {
                         .func = slog_func,
                     }};
}