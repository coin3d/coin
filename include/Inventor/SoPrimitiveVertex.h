/**************************************************************************\
 *
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SOPRIMITIVEVERTEX_H__
#define __SOPRIMITIVEVERTEX_H__

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>


class SoDetail;

class SoPrimitiveVertex
{
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

  void setMaterialIndex(const int index);
  void setDetail(SoDetail * const detail);

private:
  SbVec3f point;
  SbVec3f normal;
  SbVec4f textureCoords;
  int materialIndex;
  SoDetail * detail;
};

#endif // !__SOPRIMITIVEVERTEX_H__
