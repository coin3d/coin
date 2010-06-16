/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#ifdef HAVE_VRML97

/*!
  \class SoVRMLIndexedLine SoVRMLIndexedLine.h Inventor/VRMLnodes/SoVRMLIndexedLine.h
  \brief The SoVRMLIndexedLine class is an abstract superclass for lines specified with indices.
  \ingroup VRMLnodes

  \COININTERNAL
*/

#include <Inventor/VRMLnodes/SoVRMLIndexedLine.h>
#include "coindefs.h"

#include <stddef.h>

#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLCoordinate.h>

#include "nodes/SoSubNodeP.h"

SO_NODE_ABSTRACT_SOURCE(SoVRMLIndexedLine);

// Doc in superclass.
void
SoVRMLIndexedLine::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_ABSTRACT_CLASS(SoVRMLIndexedLine, SO_VRML97_NODE_TYPE);
}

SoVRMLIndexedLine::SoVRMLIndexedLine(void) // protected
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLIndexedLine);

  SO_VRMLNODE_ADD_EMPTY_MFIELD(coordIndex);
  SO_VRMLNODE_ADD_EMPTY_MFIELD(colorIndex);
}

SoVRMLIndexedLine::~SoVRMLIndexedLine() // virtual, protected
{
}

// Doc in superclass.
void
SoVRMLIndexedLine::computeBBox(SoAction * COIN_UNUSED_ARG(action),
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
SoVRMLIndexedLine::getNumVerts(int COIN_UNUSED_ARG(startcoord))
{
  // FIXME: why is there just a dummy implementation of this method?)
  // Please document special cases like this.  20030603 mortene.
  return 0;
}

void
SoVRMLIndexedLine::setupIndices(int COIN_UNUSED_ARG(numFaces))
{
  // FIXME: why is there just a dummy implementation of this method?
  // Please document special cases like this.  20030603 mortene.
}

const int32_t *
SoVRMLIndexedLine::getColorIndices(void) // protected
{
  // FIXME: why is there just a dummy implementation of this method?
  // Please document special cases like this.  20030603 mortene.
  return NULL;
}

void
SoVRMLIndexedLine::notify(SoNotList * list)
{
  // FIXME: why is there just a dummy implementation of this method?
  // Please document special cases like this.  20030603 mortene.
  inherited::notify(list);
}

#endif // HAVE_VRML97
