#include "engine.hpp"
#include "src/assets.hpp"
#include "src/input.hpp"
#include "src/physics.hpp"
#include "src/player.hpp"
#include "thirdparty/sokol/sokol_log.h"

static void init(void) {
  LOG_DEBUG("Debug mode!")

  physics::init();
  renderer::init();
  player::init();
}

void event(const sapp_event *event) {
  if (event->type == SAPP_EVENTTYPE_KEY_DOWN) {
    if (event->key_code == SAPP_KEYCODE_ESCAPE) {
      sapp_request_quit();
    }

    input::handle_keydown(event->key_code);
  } else if (event->type == SAPP_EVENTTYPE_KEY_UP) {
    input::handle_keyup(event->key_code);
  }
}

void frame(void) {
  const float delta_time = 1.0f/60.0f;

  input::handle_frame();

  world::main.update();
  physics::update(delta_time);
  player::update();
  renderer::frame();
}

void cleanup(void) {
  assets::finish();
  renderer::finish();
  physics::init();
}

sapp_desc sokol_main(int argc, char *argv[]) {
  return sapp_desc{
      .init_cb = init,
      .frame_cb = frame,
      .cleanup_cb = cleanup,
      .event_cb = event,
      .width = 1200,
      .height = 800,
      .sample_count = 8,
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