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

#ifndef COIN_SOTEXTURECOORDINATENORMALMAPP_H
#define COIN_SOTEXTURECOORDINATENORMALMAPP_H
#include <cfloat>
#include "nodes/SoTextureCoordinateReflectionP.h"
namespace {
// Accept a cofactor recovery only for affine matrices whose determinant
// suffered range loss, never to overturn the finite singularity threshold.
inline bool
coin_texture_normal_range(const SbMatrix & matrix, bool & admissible,
                          bool * finite_rejection = NULL)
{
  admissible = false;
  if (finite_rejection) *finite_rejection = false;
  if (matrix[0][3] != 0.0f || matrix[1][3] != 0.0f ||
      matrix[2][3] != 0.0f || matrix[3][3] != 1.0f) return false;
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (!std::isfinite(matrix[i][j])) return false;
  float pos = 0.0f, neg = 0.0f;
  bool range = false;
  const int rows[6][3] = { {0,1,2}, {1,2,0}, {2,0,1},
                           {2,1,0}, {1,0,2}, {0,2,1} };
  for (int k = 0; k < 6; ++k) {
    const float a = matrix[0][rows[k][0]], b = matrix[1][rows[k][1]],
                c = matrix[2][rows[k][2]];
    const float sign = k < 3 ? 1.0f : -1.0f;
    const float term = (sign*a)*b*c;
    const float first = (sign*a)*b;
    if (!std::isfinite(first) || (first == 0.0f && a != 0.0f && b != 0.0f) ||
        !std::isfinite(term) || (term == 0.0f && a != 0.0f && b != 0.0f && c != 0.0f)) range = true;
    if (term >= 0.0f) pos += term; else neg += term;
  }
  const float determinant = pos+neg;
  const float magnitude = pos-neg;
  const bool finite_classifier = determinant != 0.0f &&
    std::isfinite(pos) && std::isfinite(neg) &&
    std::isfinite(determinant) && std::isfinite(magnitude);
  if (!std::isfinite(pos) || !std::isfinite(neg) ||
      !std::isfinite(determinant) || !std::isfinite(magnitude)) range = true;
  // A lost product does not authorize replacing an otherwise finite Float
  // singularity decision. In particular, a tiny unrelated determinant term
  // may underflow without affecting that decision's representable inputs.
  if (finite_classifier || !range) {
    admissible = determinant != 0.0f &&
      !(std::fabs(determinant/magnitude) < FLT_EPSILON);
    if (finite_rejection && finite_classifier) *finite_rejection = !admissible;
  }
  else {
    double widepos = 0.0, wideneg = 0.0;
    for (int k = 0; k < 6; ++k) {
      const double a = matrix[0][rows[k][0]];
      const double term = (k < 3 ? a : -a)*matrix[1][rows[k][1]]*matrix[2][rows[k][2]];
      if (term >= 0.0) widepos += term; else wideneg += term;
    }
    const double determinant = widepos+wideneg;
    admissible = determinant != 0.0 &&
      !(std::fabs(determinant/(widepos-wideneg)) < FLT_EPSILON);
  }
  if (admissible && !range) {
    const float reciprocal = 1.0f / (pos+neg);
    if (!std::isfinite(reciprocal)) range = true;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        const float a = matrix[(i+1)%3][(j+1)%3], b = matrix[(i+2)%3][(j+2)%3];
        const float c = matrix[(i+1)%3][(j+2)%3], d = matrix[(i+2)%3][(j+1)%3];
        const float first = a*b, second = c*d, cofactor = first-second;
        if (!std::isfinite(first) || !std::isfinite(second) || !std::isfinite(cofactor) ||
            !std::isfinite(cofactor*reciprocal) ||
            (first == 0.0f && a != 0.0f && b != 0.0f) ||
            (second == 0.0f && c != 0.0f && d != 0.0f) ||
            (cofactor*reciprocal == 0.0f && cofactor != 0.0f && reciprocal != 0.0f)) range = true;
      }
    }
  }
  return range;
}

}
#endif
