#ifndef COIN_SBBSPTREE_H
#define COIN_SBBSPTREE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox3f.h>
#include <stddef.h> // for NULL definition

class SbSphere;

class coin_bspnode;

class COIN_DLL_API SbBSPTree {
public:
  SbBSPTree(const int maxnodepts = 64, const int initsize = 4);
  ~SbBSPTree();

  int numPoints() const;
  SbVec3f getPoint(const int idx) const;
  void getPoint(const int idx, SbVec3f & pt) const;
  void * getUserData(const int idx) const;
  void setUserData(const int idx, void * const data);

  int addPoint(const SbVec3f & pt, void * const userdata = NULL);
  int removePoint(const SbVec3f & pt);
  void removePoint(const int idx);
  int findPoint(const SbVec3f & pos) const;
  void findPoints(const SbSphere & sphere, SbList <int> & array) const;
  int findClosest(const SbSphere & sphere, SbList <int> & array) const;
  int findClosest(const SbVec3f & pos) const;
  void clear(const int initsize = 4);

  const SbBox3f & getBBox() const;
  const SbVec3f * getPointsArrayPtr() const;

private:
  friend class coin_bspnode;
  SbList <SbVec3f> pointsArray;
  SbList <void *> userdataArray;
  coin_bspnode * topnode;
  int maxnodepoints;
  SbBox3f boundingBox;
};

#endif // !COIN_SBBSPTREE_H
