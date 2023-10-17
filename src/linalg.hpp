#pragma once

#include "thirdparty/HandmadeMath/HandmadeMath.h"

constexpr void HMM_TranslateInplace(HMM_Mat4 &Left, HMM_Vec3 Translation) {
  Left.Elements[3][0] += Translation.X;
  Left.Elements[3][1] += Translation.Y;
  Left.Elements[3][2] += Translation.Z;
}

constexpr HMM_Quat HMM_QuaternionMultiplyVec3(HMM_Quat q, HMM_Vec3 v) {
  HMM_Quat Result;

  Result.W = -(q.X * v.X) - (q.Y * v.Y) - (q.Z * v.Z);
  Result.X = (q.W * v.X) + (q.Y * v.Z) - (q.Z * v.Y);
  Result.Y = (q.W * v.Y) + (q.Z * v.X) - (q.X * v.Z);
  Result.Z = (q.W * v.Z) + (q.X * v.Y) - (q.Y * v.X);

  return Result;
}

constexpr HMM_Vec3 HMM_RotateVec3(HMM_Quat q, HMM_Vec3 v) {
  HMM_Quat inverted;
  inverted.X = -q.X;
  inverted.Y = -q.Y;
  inverted.Z = -q.Z;
  inverted.W = q.W;

  inverted = HMM_NormQ(inverted);
  HMM_Quat tmp = HMM_QuaternionMultiplyVec3(q, v);
  HMM_Quat Result = tmp * inverted;

  return HMM_V3(Result.X, Result.Y, Result.Z);
}

constexpr HMM_Vec3 operator*(HMM_Quat Left, HMM_Vec3 Right) { return HMM_RotateVec3(Left, Right); }
