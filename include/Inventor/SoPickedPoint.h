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

#ifndef __SOPICKEDPOINT_H__
#define __SOPICKEDPOINT_H__

#include <Inventor/SbBasic.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec4f.h>
#include <Inventor/lists/SoDetailList.h>
#include <Inventor/SbViewportRegion.h>

class SoPath;
class SoDetail;
class SoNode;
class SoState;

class SoPickedPoint
{
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

  SbVec3f point;
  SbVec3f normal;
  SbVec4f texCoords;
  int materialIndex;
  SoPath *path;
  SbBool onGeometry;
  SoDetailList detailList;
  SoState *state;
  SbViewportRegion viewport;

  class SoGetMatrixAction *getMatrixAction() const;  
  void applyMatrixAction(const SoNode * const node) const;
};

#endif // !__SOPICKEDPOINT_H__
