#include "engine.hpp"
#include "sokol/sokol_app.h"
#include "sokol/sokol_log.h"
#include "src/assets.hpp"
#include "src/player.hpp"

static void init(void) {
  LOG_DEBUG("Debug mode!")

  renderer::init();
  player::init();
}

void event(const sapp_event *event) {
  if (event->type == SAPP_EVENTTYPE_KEY_DOWN && event->key_code == SAPP_KEYCODE_ESCAPE) {
    sapp_request_quit();
  }
}

void frame(void) {
  world::main.update();
  player::update();
  renderer::frame();
}

void cleanup(void) {
  assets::finish();
  renderer::finish();
}

sapp_desc sokol_main(int argc, char *argv[]) {
  return sapp_desc{
      .init_cb = init,
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
      .logger =
          {
              .func = slog_func,
          },
      .win32_console_attach = true,
  };
}