#include "input.hpp"
#include <cstddef>

namespace input {

static bool pressed_keys[SAPP_MAX_KEYCODES] = {};

static HMM_Vec2 axis_left = HMM_V2(0, 0);

void handle_keydown(const sapp_keycode key_code) { pressed_keys[key_code] = true; }

void handle_keyup(const sapp_keycode key_code) { pressed_keys[key_code] = false; }

void handle_frame() {
  if (pressed_keys[SAPP_KEYCODE_A]) {
    axis_left.X = -1;
  } else if (pressed_keys[SAPP_KEYCODE_D]) {
    axis_left.X = 1;
  } else {
    axis_left.X = 0;
  }

  if (pressed_keys[SAPP_KEYCODE_S]) {
    axis_left.Y = -1;
  } else if (pressed_keys[SAPP_KEYCODE_W]) {
    axis_left.Y = 1;
  } else {
    axis_left.Y = 0;
  }
}

[[nodiscard]] HMM_Vec2 get_left_axis() { return axis_left; }

} // namespace input