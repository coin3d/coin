/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
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

#include <Inventor/VRMLnodes/SoVRMLIndexedLine.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <stddef.h>

SO_NODE_ABSTRACT_SOURCE(SoVRMLIndexedLine);

void
SoVRMLIndexedLine::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLIndexedLine, SO_VRML97_NODE_TYPE);
}

SoVRMLIndexedLine::SoVRMLIndexedLine(void) // protected
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoVRMLIndexedLine);

  SO_VRMLNODE_ADD_EMPTY_MFIELD(coordIndex);
  SO_VRMLNODE_ADD_EMPTY_MFIELD(colorIndex);
}

SoVRMLIndexedLine::~SoVRMLIndexedLine() // virtual, protected
{
}

void
SoVRMLIndexedLine::computeBBox(SoAction * action,
                               SbBox3f & box,
                               SbVec3f & center)
{
  SoVRMLCoordinate * node = (SoVRMLCoordinate*) this->coord.getValue();
  if (node == NULL) return;

  int numCoords = node->point.getNum();
  const SbVec3f * coords = node->point.getValues(0);

  box.makeEmpty();
  const int32_t * ptr = coordIndex.getValues(0);
  const int32_t * endptr = ptr + coordIndex.getNum();
  while (ptr < endptr) {
    int idx = *ptr++;
    assert(idx < numCoords);
    if (idx >= 0) box.extendBy(coords[idx]);
  }
  if (!box.isEmpty()) center = box.getCenter();
}

int
SoVRMLIndexedLine::getNumVerts(int startcoord)
{
  return 0;
}

void
SoVRMLIndexedLine::setupIndices(int numFaces)
{
}

const int32_t *
SoVRMLIndexedLine::getColorIndices(void) // protected
{
  return NULL;
}

void
SoVRMLIndexedLine::notify(SoNotList * list)
{
  inherited::notify(list);
}
