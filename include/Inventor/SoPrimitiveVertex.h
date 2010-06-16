#ifndef COIN_SOPRIMITIVEVERTEX_H
#define COIN_SOPRIMITIVEVERTEX_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/system/inttypes.h>

class SoDetail;

class COIN_DLL_API SoPrimitiveVertex {
public:
  SoPrimitiveVertex(void);
  SoPrimitiveVertex(const SoPrimitiveVertex & pv);
  ~SoPrimitiveVertex(void);

  SoPrimitiveVertex & operator = (const SoPrimitiveVertex & pv);

  const SbVec3f & getPoint(void) const { return point; }
  const SbVec3f & getNormal(void) const { return normal; }
  const SbVec4f & getTextureCoords(void) const { return textureCoords; }
  int getMaterialIndex(void) const { return materialIndex; }
  uint32_t getPackedColor(void) const { return packedColor; }
  const SoDetail * getDetail(void) const { return detail; }

  void setPoint(const SbVec3f & pt) { point = pt; }
  void setPoint(float x, float y, float z) { point.setValue(x, y, z); }
  void setNormal(const SbVec3f & n) { normal = n; }
  void setNormal(float nx, float ny, float nz) { normal.setValue(nx, ny, nz); }
  void setTextureCoords(const SbVec2f & tex) { textureCoords.setValue(tex[0], tex[1], 0.0f, 1.0f); }
  void setTextureCoords(float tx, float ty) { textureCoords.setValue(tx, ty, 0.0f, 1.0f); }
  void setTextureCoords(const SbVec3f & tex) { textureCoords.setValue(tex[0], tex[1], tex[2], 1.0f); }
  void setTextureCoords(float tx, float ty, float tz) { textureCoords.setValue(tx, ty, tz, 1.0f); }
  void setTextureCoords(const SbVec4f & tex) { textureCoords = tex; }
  void setTextureCoords(float tx, float ty, float tz, float tw) { textureCoords.setValue(tx, ty, tz, tw); }

  void setMaterialIndex(int index) { materialIndex = index; }
  void setPackedColor(uint32_t rgba) { packedColor = rgba; }
  void setDetail(SoDetail * d) { detail = d; }

private:
  SbVec3f point;
  SbVec3f normal;
  SbVec4f textureCoords;
  int materialIndex;
  SoDetail * detail;
  uint32_t packedColor;

}; // SoPrimitiveVertex

#endif // !COIN_SOPRIMITIVEVERTEX_H
