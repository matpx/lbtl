#include "sokol/sokol_app.h"
#include "sokol/sokol_log.h"

#include "renderer.hpp"
#include "src/player.hpp"
#include "src/world.hpp"

static void init(void) {
  renderer::init();
  player::init();
}

void event(const sapp_event *event) {
  if (event->type == SAPP_EVENTTYPE_KEY_DOWN &&
      event->key_code == SAPP_KEYCODE_ESCAPE) {
    sapp_request_quit();
  }
}

void frame(void) {
  world::main.update();
  renderer::frame();
}

void cleanup(void) { renderer::finish(); }

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