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

#ifndef __SBBSPTREE_H__
#define __SBBSPTREE_H__

#include <Inventor/lists/SbList.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbBox3f.h>
#include <stddef.h> // for NULL definition

class SbSphere;

class coin_bspnode;

class SbBSPTree
{
public:
  SbBSPTree(const int maxnodepts = 64, const int initsize = 4);
  ~SbBSPTree();

  int numPoints() const;
  SbVec3f getPoint(const int idx);
  void getPoint(const int idx, SbVec3f & pt);
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

#endif // !__SBBSPTREE_H__
