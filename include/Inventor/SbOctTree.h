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
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef _SBOCTTREE_H_
#define _SBOCTTREE_H_

#include <Inventor/SbVec3f.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBox3f.h>

class SbSphere;
class SbOctTreeNode;
class SbPlane;

typedef struct
{
  void (*bboxfunc)(void * const item, SbBox3f & bbox);
  SbBool (*ptinsidefunc)(void * const item, const SbVec3f & pt);
  SbBool (*insideboxfunc)(void * const item, const SbBox3f & box);
  SbBool (*insidespherefunc)(void * const item, const SbSphere & sphere);
  SbBool (*insideplanesfunc)(void * const item,
                             const SbPlane * const planes,
                             const int numplanes);
} SbOctTreeFuncs;


class COIN_DLL_EXPORT SbOctTree {
public:
  SbOctTree(const SbBox3f & bbox,
            const SbOctTreeFuncs & itemfuncs,
            const int maxitemspernode = 64);
  ~SbOctTree();

  void addItem(void * const item);
  void removeItem(void * const item);
  void findItems(const SbVec3f & pos,
                 SbList <void*> & destarray,
                 const SbBool removeduplicates = TRUE) const;
  void findItems(const SbBox3f & box,
                 SbList <void*> & destarray,
                 const SbBool removeduplicates = TRUE) const;
  void findItems(const SbSphere & sphere,
                 SbList <void*> & destarray,
                 const SbBool removeduplicates = TRUE) const;
  void findItems(const SbPlane * const planes,
                 const int numplanes,
                 SbList <void*> & destarray,
                 const SbBool removeduplicates= TRUE) const;

  const SbBox3f & getBoundingBox(void) const;
  void clear(void);

private:
  friend class SbOctTreeNode;
  SbBox3f boundingbox;
  SbOctTreeNode * topnode;
  SbOctTreeFuncs itemfuncs;
  int maxitemspernode;
};


#endif // _SBOCTTREE_H_
