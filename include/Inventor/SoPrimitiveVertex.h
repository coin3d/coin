/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

#ifndef COIN_SOPRIMITIVEVERTEX_H
#define COIN_SOPRIMITIVEVERTEX_H

#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>


class SoDetail;

class COIN_DLL_API SoPrimitiveVertex {
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
