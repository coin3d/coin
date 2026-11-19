/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#ifndef COIN_SOTEXTURECOORDINATEMATHP_H
#define COIN_SOTEXTURECOORDINATEMATHP_H
#include <cmath>
#include <cfloat>
namespace {
// A short compensated sum is needed only after a result loses its range
// or collapses exactly to zero. It does not replace finite nonzero sums.
inline double coin_texture_sum(const double * terms, int count)
{
  double sum = 0.0, correction = 0.0;
  for (int i = 0; i < count; ++i) {
    const double next = sum + terms[i];
    correction += std::fabs(sum) >= std::fabs(terms[i]) ?
      (sum-next)+terms[i] : (terms[i]-next)+sum;
    sum = next;
  }
  return sum+correction;
}

inline float coin_texture_dot(float legacy, const float * a, const float * b, int count)
{
  bool recover = legacy == 0.0f || !std::isfinite(legacy);
  for (int i = 0; i < count; ++i) {
    if (!std::isfinite(a[i]) || !std::isfinite(b[i])) return legacy;
    const float product = a[i]*b[i];
    if (!std::isfinite(product) ||
        (product == 0.0f && a[i] != 0.0f && b[i] != 0.0f)) recover = true;
  }
  if (!recover) return legacy;
  double terms[4];
  for (int i = 0; i < count; ++i) terms[i] = double(a[i])*b[i];
  const double result = coin_texture_sum(terms, count);
  if (std::isfinite(result) && std::fabs(result) <= FLT_MAX &&
      ((result != 0.0 && float(result) != 0.0f) ||
       (result == 0.0 && !std::isfinite(legacy)))) return float(result);
  return legacy;
}

inline float coin_texture_longitude(float legacy, float x, float z)
{
  // A platform atan2 can round to a value just inside the seam.
  const bool nearLowerEdge = x < 0.0f && legacy <= FLT_EPSILON * 0.5f;
  if ((nearLowerEdge || legacy >= 1.0f) && x != 0.0f && z < 0.0f &&
      std::isfinite(x) && std::isfinite(z)) {
    const double edge = std::atan2(std::fabs(double(x)), -double(z)) /
      (2.0 * 3.14159265358979323846);
    const float recovered = float(x < 0.0f ? edge : 1.0-edge);
    // Keep finite nonzero results unless rounding swamps the small angle.
    if (recovered > 0.0f && recovered <= 1.0f &&
        (legacy <= 0.0f || legacy >= 1.0f || edge * 2.0 < legacy)) return recovered;
  }
  return legacy;
}

inline float coin_texture_box_ratio(float point, float minimum, float maximum)
{
  float size = maximum - minimum;
  if (size == 0.0f) size = 1.0f;
  const float numerator = point - minimum;
  const float legacy = numerator / size;
  if ((!std::isfinite(size) || !std::isfinite(numerator)) &&
      std::isfinite(point) && std::isfinite(minimum) && std::isfinite(maximum) && maximum >= minimum) {
    const double denominator = maximum == minimum ? 1.0 : double(maximum)-minimum;
    const double recovered = (double(point)-minimum) / denominator;
    if (std::isfinite(recovered) && std::fabs(recovered) <= FLT_MAX) return float(recovered);
  }
  return legacy;
}
}
#endif
