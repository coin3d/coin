#ifndef COIN_SBOCTTREE_H
#define COIN_SBOCTTREE_H

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

#include <Inventor/SbVec3f.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbBox3f.h>
#include <stdio.h>

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


class COIN_DLL_API SbOctTree {
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
  void debugTree(FILE * fp);

private:
  friend class SbOctTreeNode;
  SbBox3f boundingbox;
  SbOctTreeNode * topnode;
  SbOctTreeFuncs itemfuncs;
  int maxitemspernode;
};


#endif // !COIN_SBOCTTREE_H
