/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOPRIMITIVEVERTEX_H
#define COIN_SOPRIMITIVEVERTEX_H

#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>


class SoDetail;

class COIN_DLL_EXPORT SoPrimitiveVertex {
public:
  SoPrimitiveVertex(void);
  SoPrimitiveVertex(const SoPrimitiveVertex & pv);
  ~SoPrimitiveVertex();
  const SbVec3f & getPoint(void) const;
  const SbVec3f & getNormal(void) const;
  const SbVec4f & getTextureCoords(void) const;
  int getMaterialIndex(void) const;
  const SoDetail * getDetail(void) const;
  SoPrimitiveVertex & operator =(const SoPrimitiveVertex & pv);

  void setPoint(const SbVec3f & point);
  void setNormal(const SbVec3f & normal);
  void setTextureCoords(const SbVec4f & texcoords);
  void setTextureCoords(const SbVec2f & texcoords);

  void setMaterialIndex(const int index);
  void setDetail(SoDetail * const detail);


private:
  SbVec3f point;
  SbVec3f normal;
  SbVec4f textureCoords;
  int materialIndex;
  SoDetail * detail;
};

#endif // !COIN_SOPRIMITIVEVERTEX_H
