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

#ifndef COIN_SOVERTEXARRAYTEXCOORDGUARD_H
#define COIN_SOVERTEXARRAYTEXCOORDGUARD_H

#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoMultiTextureCoordinateElement.h>
#include <Inventor/elements/SoMultiTextureEnabledElement.h>
#include <Inventor/misc/SoState.h>
#include <cstddef>
#include <cstdint>
#include <Inventor/threads/SbMutex.h>
#include <memory>
#include <mutex>

class SoShape;
class SoAction;
class SbBox3f;
class SoTextureCoordinateBundle;
class SoVertexArrayTexCoordCache;

/*!
  \class SoVertexArrayTexCoordGuard
  \brief Keeps cached default texture coordinates alive during array drawing.

  Capture isDefaultMapping() before constructing SoTextureCoordinateBundle,
  which replaces the default mapping with a callback. After constructing the
  bundle, canUseArrays() checks that each active unit has coordinates or uses
  GL texgen. Only the default mapping of a single 2D texture on unit 0 can be
  materialized here; arbitrary functions may depend on per-corner normals.

  Coordinates are reused while the shape, coordinate source and default
  mapping are unchanged. The cache owns the array and an optional VBO, whose
  context resources are managed by SoVBO. Active draws retain their storage
  even when another traversal replaces the shape's cache. Access to a shared
  cache's VBO is serialized until drawing and state restoration finish.
  Default bounding boxes use a separate cache which tracks the state elements
  read by computeBBox(), including dependencies inherited from the scene.

  Callers apply isAffordable() before materializing default coordinates.
  Its existing byte budget and reference ratio also bound this cache's entry;
  storage uses SbVec2f rather than the temporary path's SbVec4f. Each shape
  retains one entry until replacement, notification or destruction. This is
  a per-shape policy, not a global CPU or GPU memory budget.

  Construct the guard inside the vertex-array draw scope, after the bundle,
  and call finishVertexArray() before destroying it. Its destructor restores
  the texture-coordinate state before the bundle is destroyed.
*/
class SoVertexArrayTexCoordGuard {
public:
  /*! Returns whether materializing the complete coordinate element has a
      bounded cost relative to the vertices submitted by this draw. */
  static SbBool isAffordable(int numcoords, std::int64_t numreferenced)
  {
    static const std::size_t maxTemporaryBytes =
      std::size_t(32) * 1024 * 1024;
    const std::size_t maxcoords = maxTemporaryBytes / sizeof(SbVec4f);
    // Equivalent to numcoords <= 2*numreferenced + 64 without multiplying.
    return numcoords >= 0 && numreferenced >= 0 &&
      std::size_t(numcoords) <= maxcoords &&
      (numcoords <= 64 || numreferenced >= (std::int64_t(numcoords) - 64 + 1) / 2);
  }

  /*! Returns object-space bounds for default mapping during GL rendering.
      The cached result is checked against shape notifications and the state
      elements read by computeBBox(). Its dependencies are propagated to any
      enclosing render cache. */
  static void getDefaultBBox(SoAction * action, SoShape * shape,
                            SbBox3f & box, SbVec3f & center);

  /*! Tests for a single enabled 2D texture on unit 0 with DEFAULT coordinates.
      Call before the bundle installs its default-coordinate function. */
  static SbBool isDefaultMapping(SoState * state)
  {
    int lastenabled = -1;
    const SbBool * enabled =
      SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
    return lastenabled == 0 && enabled && enabled[0] &&
      SoMultiTextureEnabledElement::getMode(state, 0) ==
        SoMultiTextureEnabledElement::TEXTURE2D &&
      SoMultiTextureCoordinateElement::getType(state, 0) ==
        SoMultiTextureCoordinateElement::DEFAULT;
  }

  /*! Checks that enabled units have arrays or GL texgen, allowing the
      previously captured single default mapping to be materialized.
      Bindings, index compatibility and array lengths remain the caller's
      responsibility. */
  static SbBool canUseArrays(SoState * state, SbBool defaultmapping)
  {
    int lastenabled = -1;
    const SbBool * enabled =
      SoMultiTextureEnabledElement::getEnabledUnits(state, lastenabled);
    if (!enabled) return TRUE;
    const SoMultiTextureCoordinateElement * element =
      SoMultiTextureCoordinateElement::getInstance(state);
    for (int unit = 0; unit <= lastenabled; ++unit) {
      if (enabled[unit] && !element->getNum(unit) &&
          element->getType(unit) != SoMultiTextureCoordinateElement::TEXGEN &&
          !(defaultmapping && unit == 0 && lastenabled == 0)) return FALSE;
    }
    return TRUE;
  }

  /*! Installs a cached coordinate array on a pushed state when
      generatedefault is TRUE. The bundle must describe the single default
      2D mapping and remain alive through the draw. VBOs are used only when
      the current context and vertex-count policy allow them. */
  SoVertexArrayTexCoordGuard(SoState * state, SoShape * shape,
                           const SoCoordinateElement * coords,
                           const SoTextureCoordinateBundle & bundle,
                           SbBool generatedefault);

  /*! Restores the coordinate and VBO elements, then releases the draw's
      lock and ownership of its coordinate storage. */
  ~SoVertexArrayTexCoordGuard();

private:
  SoVertexArrayTexCoordGuard(const SoVertexArrayTexCoordGuard &);
  SoVertexArrayTexCoordGuard & operator=(const SoVertexArrayTexCoordGuard &);
  SoState * state;
  SbBool pushed;
  std::shared_ptr<SoVertexArrayTexCoordCache> cache;
  std::unique_lock<SbMutex> locked;
};

#endif // COIN_SOVERTEXARRAYTEXCOORDGUARD_H
