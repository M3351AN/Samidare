// Copyright (c) 2025 渟雲. All rights reserved.
//
// Licensed under the TOSSRCU 2025.9 License (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  https://raw.githubusercontent.com/M3351AN/M3351AN/9e7630a8511b8306c62952ca1a4f1ce0cc5b784a/LICENSE
//
// -----------------------------------------------------------------------------
// File: maths.h
// Author: 渟雲(quq[at]outlook.it)
// Date: 2025-09-30
//
// Description:
//   This file contains math functions define for using in Samidare.
//
// -----------------------------------------------------------------------------
#pragma once
#ifndef MATHS_H_
#define MATHS_H_

// convert angle in degrees to radians
#define M_DEG2RAD(DEGREES) ((DEGREES) * (MATH::_PI / 180.f))
// convert angle in radians to degrees
#define M_RAD2DEG(RADIANS) ((RADIANS) * (180.f / MATH::_PI))
/// linearly interpolate the value between @a'X0' and @a'X1' by @a'FACTOR'
#define M_LERP(X0, X1, FACTOR) ((X0) + ((X1) - (X0)) * (FACTOR))
/// trigonometry
#define M_COS(ANGLE) cos(ANGLE)
#define M_SIN(ANGLE) sin(ANGLE)
#define M_TAN(ANGLE) tan(ANGLE)
/// power
#define M_POW(BASE, EXPONENT) pow(BASE, EXPONENT)
/// absolute value
#define M_ABS(VALUE) abs(VALUE)
/// square root
#define M_SQRT(VALUE) sqrt(VALUE)
/// floor
#define M_FLOOR(VALUE) floor(VALUE)

/*
 * MATHEMATICS
 * - basic trigonometry, algebraic mathematical functions and constants
 */
namespace MATH {
/* @section: constants */
// pi value
inline constexpr float _PI = 3.141592654f;
// double of pi
inline constexpr float _2PI = 6.283185307f;
// half of pi
inline constexpr float _HPI = 1.570796327f;
// quarter of pi
inline constexpr float _QPI = 0.785398163f;
// reciprocal of double of pi
inline constexpr float _1DIV2PI = 0.159154943f;
// golden ratio
inline constexpr float _PHI = 1.618033988f;

[[nodiscard]] inline float normalize_yaw(float yaw) noexcept {
  while (yaw > 180.0f) yaw -= 360.0f;

  while (yaw < -180.0f) yaw += 360.0f;

  return yaw;
}

template <typename T>
inline T clamp(const T& n, const T& lower, const T& upper) {
  return Max(lower, Min(n, upper));
}

/* @section: exponential */
/// @returns: true if given number is power of two, false otherwise
template <typename T>
  requires(std::is_integral_v<T>)
[[nodiscard]] inline constexpr bool IsPowerOfTwo(const T value) noexcept {
  return value != 0 && (value & (value - 1)) == 0;
}
}  // namespace MATH
#endif
