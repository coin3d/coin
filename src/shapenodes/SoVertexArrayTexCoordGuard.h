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
#include <vector>

/*!
  \class SoVertexArrayTexCoordGuard
  \brief Keeps temporary default texture coordinates alive during array drawing.

  Capture isDefaultMapping() before constructing SoTextureCoordinateBundle,
  which replaces the default mapping with a callback. After constructing the
  bundle, canUseArrays() checks that each active unit has coordinates or uses
  GL texgen. Only the default mapping of a single 2D texture on unit 0 can be
  materialized here; arbitrary functions may depend on per-corner normals.

  Construct the guard inside the vertex-array draw scope, after the bundle,
  and call finishVertexArray() before destroying it. Its destructor restores
  the texture-coordinate state before the bundle is destroyed.
*/
class SoVertexArrayTexCoordGuard {
public:
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

  SoVertexArrayTexCoordGuard(SoState * state, SoNode * node,
                           const SoCoordinateElement * coords,
                           SbBool generatedefault)
    : state(state), pushed(FALSE)
  {
    if (!generatedefault) return;
    const SoMultiTextureCoordinateElement * element =
      SoMultiTextureCoordinateElement::getInstance(state);
    const int count = coords->getNum();
    this->coordinates.reserve(count);
    const SbVec3f normal(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < count; ++i) {
      this->coordinates.push_back(element->get(0, coords->get3(i), normal));
    }
    state->push();
    this->pushed = TRUE;
    SoMultiTextureCoordinateElement::set4(state, node, 0, count,
                                          this->coordinates.data());
  }

  ~SoVertexArrayTexCoordGuard()
  {
    if (this->pushed) this->state->pop();
  }

private:
  SoVertexArrayTexCoordGuard(const SoVertexArrayTexCoordGuard &);
  SoVertexArrayTexCoordGuard & operator=(const SoVertexArrayTexCoordGuard &);
  SoState * state;
  SbBool pushed;
  std::vector<SbVec4f> coordinates;
};

#endif // COIN_SOVERTEXARRAYTEXCOORDGUARD_H
