#pragma once

#include "HandmadeMath/HandmadeMath.h"

constexpr void HMM_TranslateInplace(HMM_Mat4 &Left, HMM_Vec3 Translation) {
  Left.Elements[3][0] += Translation.X;
  Left.Elements[3][1] += Translation.Y;
  Left.Elements[3][2] += Translation.Z;
}
