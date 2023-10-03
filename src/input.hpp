#pragma once

#include "linalg.hpp"
#include "src/thirdparty/HandmadeMath/HandmadeMath.h"
#include "thirdparty/sokol/sokol_app.h"

namespace input {

void handle_keydown(const sapp_keycode key_code);
void handle_keyup(const sapp_keycode key_code);

void handle_frame();

[[nodiscard]] HMM_Vec2 get_left_axis();

} // namespace input
