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

#ifndef COIN_SO_PICKED_POINT_H
#define COIN_SO_PICKED_POINT_H

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/lists/SoDetailList.h>
#include <Inventor/SbViewportRegion.h>

class SoPath;
class SoDetail;
class SoNode;
class SoState;
class SbMatrix;

class COIN_DLL_API SoPickedPoint {
public:
  SoPickedPoint(const SoPickedPoint & pp);
  SoPickedPoint(const SoPath * const path, SoState * const state,
                const SbVec3f & objSpacePoint);
  ~SoPickedPoint();
  SoPickedPoint *copy() const;
  const SbVec3f &getPoint() const;
  const SbVec3f &getNormal() const;
  const SbVec4f &getTextureCoords() const;
  int getMaterialIndex() const;
  SoPath *getPath() const;
  SbBool isOnGeometry() const;
  const SoDetail *getDetail(const SoNode * const node = NULL) const;
  const SbMatrix &getObjectToWorld(const SoNode * const node = NULL) const;
  const SbMatrix &getWorldToObject(const SoNode * const node = NULL) const;
  const SbMatrix &getObjectToImage(const SoNode * const node = NULL) const;
  const SbMatrix &getImageToObject(const SoNode * const node = NULL) const;
  SbVec3f getObjectPoint(const SoNode * const node = NULL) const;
  SbVec3f getObjectNormal(const SoNode * const node = NULL) const;
  SbVec4f getObjectTextureCoords(const SoNode * const node = NULL) const;

  void setObjectNormal(const SbVec3f &normal);
  void setObjectTextureCoords(const SbVec4f &texCoords);
  void setMaterialIndex(const int index);
  void setDetail(SoDetail * detail, SoNode * node);

private:

  SbVec3f point, objPoint;
  SbVec3f normal, objNormal;
  SbVec4f texCoords, objTexCoords;
  int materialIndex;
  SoPath *path;
  SbBool onGeometry;
  SoDetailList detailList;
  SoState *state;
  SbViewportRegion viewport;

  class SoGetMatrixAction *getMatrixAction() const;
  void applyMatrixAction(const SoNode * const node) const;
};

#endif // !COIN_SOPICKEDPOINT_H
