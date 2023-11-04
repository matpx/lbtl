#pragma once

#include "linalg.hpp"
#include "thirdparty/HandmadeMath/HandmadeMath.h"
#include "thirdparty/sokol/sokol_app.h"

namespace input {

void handle_keydown(const sapp_keycode key_code);
void handle_keyup(const sapp_keycode key_code);
void handle_mousemove(const HMM_Vec2 mouse_delta);

void pre_frame();
void post_frame();

[[nodiscard]] HMM_Vec2 get_left_axis();
[[nodiscard]] HMM_Vec2 get_right_axis();

} // namespace input
