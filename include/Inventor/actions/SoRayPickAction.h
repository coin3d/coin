/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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

#ifndef COIN_SORAYPICKACTION_H
#define COIN_SORAYPICKACTION_H

#include <Inventor/actions/SoPickAction.h>

class SbBox3f;
class SbLine;
class SbMatrix;
class SbVec2f;
class SbVec2s;
class SbVec3f;
class SbViewVolume;
class SbViewportRegion;
class SoPickedPoint;
class SoPickedPointList;


class COIN_DLL_API SoRayPickAction : public SoPickAction {
  typedef SoPickAction inherited;

  SO_ACTION_HEADER(SoRayPickAction);

public:
  SoRayPickAction(const SbViewportRegion & viewportregion);
  virtual ~SoRayPickAction();
  static void initClass(void);

  void setPoint(const SbVec2s & viewportPoint);
  void setNormalizedPoint(const SbVec2f & normpoint);
  void setRadius(const float radiusinpixels);
  void setRay(const SbVec3f & start, const SbVec3f & direction,
              float neardistance = -1.0,
              float fardistance = -1.0);
  void setPickAll(const SbBool flag);
  SbBool isPickAll(void) const;
  const SoPickedPointList & getPickedPointList(void) const;
  SoPickedPoint * getPickedPoint(const int index = 0) const;


  void computeWorldSpaceRay(void);
  SbBool hasWorldSpaceRay(void) const;
  void setObjectSpace(void);
  void setObjectSpace(const SbMatrix & matrix);
  SbBool intersect(const SbVec3f & v0, const SbVec3f & v1, const SbVec3f & v2,
                   SbVec3f & intersection, SbVec3f & barycentric,
                   SbBool & front) const;
  SbBool intersect(const SbVec3f & v0, const SbVec3f & v1,
                   SbVec3f & intersection) const;
  SbBool intersect(const SbVec3f & point) const;
  SbBool intersect(const SbBox3f & box, const SbBool usefullviewvolume = TRUE);
  SbBool intersect(const SbBox3f & box, SbVec3f & intersection,
                   const SbBool usefullviewvolume = TRUE);
  const SbViewVolume & getViewVolume(void);
  const SbLine & getLine(void);
  SbBool isBetweenPlanes(const SbVec3f & intersection) const;
  SoPickedPoint * addIntersection(const SbVec3f & objectspacepoint);

protected:
  virtual void beginTraversal(SoNode * node);

private:
  class SoRayPickActionP * pimpl;
};

#endif // !COIN_SORAYPICKACTION_H
