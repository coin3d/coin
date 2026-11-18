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

#ifndef COIN_SOTEXTURECOORDINATEREFLECTIONP_H
#define COIN_SOTEXTURECOORDINATEREFLECTIONP_H

#include <cmath>
#include "nodes/SoTextureCoordinateMathP.h"

#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>

namespace {

// Scale before squaring, including for very large or very small finite inputs.
inline void
coin_reflection_normalize(double v[3])
{
  double scale = std::fabs(v[0]);
  for (int i = 1; i < 3; i++) {
    if (std::fabs(v[i]) > scale) scale = std::fabs(v[i]);
  }
  if (scale == 0.0) return;
  for (int i = 0; i < 3; i++) v[i] /= scale;
  const double length = std::sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
  for (int i = 0; i < 3; i++) v[i] /= length;
}

inline void
coin_reflection_transform_point(const SbMatrix & matrix, double point[3])
{
  double result[3];
  for (int i = 0; i < 3; i++) {
    const double terms[4] = { point[0]*double(matrix[0][i]),
      point[1]*double(matrix[1][i]), point[2]*double(matrix[2][i]),
      double(matrix[3][i]) };
    result[i] = terms[0] + terms[1] + terms[2] + terms[3];
    if (result[i] == 0.0) {
      bool finite = std::isfinite(point[0]) && std::isfinite(point[1]) && std::isfinite(point[2]);
      for (int row = 0; row < 4; ++row)
        for (int column = 0; column < 4; ++column)
          if (!std::isfinite(matrix[row][column])) finite = false;
      if (finite) {
        const double recovered = coin_texture_sum(terms, 4);
        if (std::isfinite(recovered) && recovered != 0.0) result[i] = recovered;
      }
    }
  }
  for (int i = 0; i < 3; i++) point[i] = result[i];
}

inline void
coin_reflection_transform_normal(const SbMatrix & matrix, double normal[3],
                                 bool recoverzero = false)
{
  // For row vectors the inverse transpose is the cofactor matrix divided
  // by its determinant. Its magnitude cancels during normalization; retain
  // its sign so reflected coordinate systems keep the correct orientation.
  // Computing this in double also avoids the float inverse's overflow and
  // underflow for finite, nonsingular affine transformations.
  double cofactor[3][3];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cofactor[i][j] =
        double(matrix[(i+1)%3][(j+1)%3])*double(matrix[(i+2)%3][(j+2)%3]) -
        double(matrix[(i+1)%3][(j+2)%3])*double(matrix[(i+2)%3][(j+1)%3]);
    }
  }
  const double determinant = double(matrix[0][0])*cofactor[0][0] +
                             double(matrix[0][1])*cofactor[0][1] +
                             double(matrix[0][2])*cofactor[0][2];
  const double sign = determinant < 0.0 ? -1.0 : 1.0;
  double result[3];
  for (int i = 0; i < 3; i++) {
    const double terms[3] = { normal[0]*cofactor[0][i],
      normal[1]*cofactor[1][i], normal[2]*cofactor[2][i] };
    result[i] = sign * (terms[0] + terms[1] + terms[2]);
    if (recoverzero && result[i] == 0.0 && determinant != 0.0 &&
        std::isfinite(determinant) && std::isfinite(normal[0]) &&
        std::isfinite(normal[1]) && std::isfinite(normal[2])) {
      bool finite = true;
      for (int row = 0; row < 4; ++row)
        for (int column = 0; column < 4; ++column)
          if (!std::isfinite(matrix[row][column])) finite = false;
      if (finite) {
        const double recovered = sign * coin_texture_sum(terms, 3);
        if (std::isfinite(recovered) && recovered != 0.0) result[i] = recovered;
      }
    }
  }
  for (int i = 0; i < 3; i++) normal[i] = result[i];
  coin_reflection_normalize(normal);
}

// Keep the historical float calculation for inputs with no defined
// incident direction. This is a compatibility path, not a reflection model.
inline void
coin_texture_legacy_reflection(SoState * state, const SbVec3f & normal,
                               double reflection[3])
{
  SbVec3f wn;
  SoModelMatrixElement::get(state).multDirMatrix(normal, wn);
  SbVec3f u = normal;
  u.normalize();
  wn.normalize();
  SbVec3f r = u - SbVec3f(2.0f*wn[0]*wn[0]*u[0],
                          2.0f*wn[1]*wn[1]*u[1],
                          2.0f*wn[2]*wn[2]*u[2]);
  r.normalize();
  for (int i = 0; i < 3; ++i) reflection[i] = r[i];
}

// Return whether the historical float projection must also be retained.
inline bool
coin_texture_reflection(SoState * state, const SbVec3f & point,
                        const SbVec3f & normal, double reflection[3])
{
  if (normal[0] == 0.0f && normal[1] == 0.0f && normal[2] == 0.0f) {
    coin_texture_legacy_reflection(state, normal, reflection);
    return true;
  }
  // Fetch matrices at evaluation time: the generator may precede transforms
  // in a group, and the same node can be traversed with different cameras.
  const SbMatrix & model = SoModelMatrixElement::get(state);
  const SbMatrix & view = SoViewingMatrixElement::get(state);
  double incident[3] = { double(point[0]), double(point[1]), double(point[2]) };
  double eyeNormal[3] = { double(normal[0]), double(normal[1]), double(normal[2]) };
  // Applying model then view is Coin's row-vector model * view convention.
  // Keep the intermediates in double instead of forming a float product.
  coin_reflection_transform_point(model, incident);
  coin_reflection_transform_point(view, incident);
  if (incident[0] == 0.0 && incident[1] == 0.0 && incident[2] == 0.0) {
    coin_texture_legacy_reflection(state, normal, reflection);
    return true;
  }
  coin_reflection_normalize(incident);
  coin_reflection_normalize(eyeNormal);
  coin_reflection_transform_normal(model, eyeNormal, true);
  coin_reflection_transform_normal(view, eyeNormal, true);
  const double dot = eyeNormal[0]*incident[0] +
                     eyeNormal[1]*incident[1] + eyeNormal[2]*incident[2];
  for (int i = 0; i < 3; i++) {
    reflection[i] = incident[i] - 2.0*dot*eyeNormal[i];
  }
  return false;
}

} // namespace

#endif // COIN_SOTEXTURECOORDINATEREFLECTIONP_H
